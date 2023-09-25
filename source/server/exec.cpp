/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshintak <bshintak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 19:22:13 by fbarros           #+#    #+#             */
/*   Updated: 2023/09/24 20:41:43 by bshintak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>
#include <cstdlib>

void Server::exec(Client& client, const tokenPair& message)
{
    if (!client.isPassActive())
        return ;

    CommandMap::iterator itCommand = _commands.find(message.first);

    if (itCommand != _commands.end())
    {
        exec_ptr command = itCommand->second;
        (this->*command)(client, message.second);
    }
    else
    {
        replyNoChannelJoined(client);
    }
}

void Server::execUSER(Client& client, const std::string line)
{
    // parse line
    const std::string name = line.substr(0, line.find(' '));
    if (name.empty())
    {
        // send error
        addMessage("Missing argument.\n", client.getId());
        return ;
    }
    addMessage("Username " + name + " set.\n", client.getId());
    client.setUserActive();
    client.setUsername(name);
}

void Server::execNICK(Client& client, const std::string line)
{
    // parse line
    const std::string name = line.substr(0, line.find(' '));
    if (name.empty())
    {
        // send error
        addMessage("Missing argument.\n", client.getId());
        return ;
    }

    for (ConnectionsList::iterator it = _connections.begin(); it != _connections.end(); it++)
    {
        if (it->getNickname() == name)
        {
            // send error
            replyNickCollision(client);
            return ;
        }
    }

    client.setNickname(name);
    client.setNickActive();
    if (client.isValid())
        replyWelcome(client);
}

void Server::execQUIT(Client& client, const std::string line)
{
    const std::string nick(client.getNickname()); 
    deleteClient(client.getId());
    replyAway(nick, line);
}

void Server::execPRIVMSG(Client& client, const std::string line)
{
    std::string token(line.substr(0, line.find(' ')));
    const std::string message(line.substr(line.find(' ') + 1));
    
    if (line.empty())
    {
        addMessage(" PRIVMSG :<target>[,<target>]+ :<message>\r\n", client.getId());
    }

    if (token.at(0) == '#')
    {
        token.erase(0u, 1u); // remove '#'
        ChannelsList::const_iterator channelIt(getChannel(token));
        if (channelIt == _channels.end())
        {
            replyNoSuchNick(client, token);
        }
        else if (!channelIt->isInChannel(client.getId()))
        {
            replyNoExternalChannelMessage(client, token);
        }
        else
        {
            channelPrivateMessage(client, token, message);
        }
    }
    else
    {
        clientPrivateMessage(client, token, message);
    }
}

void Server::execNOTICE(Client& client, const std::string line)
{
    std::string token(line.substr(0, line.find(' ')));
    const std::string message(line.substr(line.find(' ') + 1));

    if (token.empty())
    {
        addMessage("Usage: NOTICE <nick/channel> <message>, sends a notice\r\n", client.getId());
    }
    else if (token.at(0) == '#')
    {
        token.erase(0, 1); // remove '#'
        ChannelsList::const_iterator channelIt(getChannel(token));
        if (channelIt == _channels.end())
        {
            replyNoSuchNick(client, token);
        }
        else if (!channelIt->isInChannel(client.getId()))
        {
            replyNotOnChannel(client, channelIt->getName());
        }
        else
        {
            channelNotice(client, token, message);
        }
    }
    else
    {
        ConnectionsList::const_iterator newClient = getClient(token);

        if (newClient == _connections.end())
        {
            replyNoSuchNick(client, token);
        }
        else
        {
            replyNoticePriv(client, message, token, *newClient);
        }
    }
}

/**
 * CHANNEL specific commands
 * */
void Server::execJOIN(Client& client, const std::string line)
{
    std::string channelName;
    std::istringstream iss(line);
    iss >> channelName;

    std::transform(channelName.begin(), channelName.end(), channelName.begin(), ::tolower);

    if (channelName.empty())
    {
        addMessage("Usage: JOIN <channel>, joins the channel\r\n", client.getId());
    }
    else if (channelName[0] != '#')
    {
        replyBadJoin(client, channelName);
    }
    else
    {
        channelName.erase(0u, 1u);
        // look for channel
        ChannelsList::iterator channelIt = getChannel(channelName);

        // if no channel, create one
        if (channelIt == _channels.end())
        {
            _channels.push_back(channelName);
            _channels.back().addClient(client, true);
            replyJoin(client.getId(), client, _channels.back());
        }
        else if (!channelIt->isClientInChannel(client.getId())) // if client is not yet in channel
        {
            Channel& channel(*channelIt);
            const ClientMap& list(channel.getClients());

            if (channel.isInviteOnly() && !client.wasInvited(channelName))
            {
                replyInviteOnly(client, channelName);
                return ;
            }
            if (!channel.addClient(client)) // add user to channel
            {
                replyChannelIsFull(client, channelName);
                return ;
            }
            // Broadcast to all channel users
            for (ClientMap::const_iterator clientIt = list.begin(); clientIt != list.end(); clientIt++)
            {
                const Client& user(*(clientIt->second));

                replyJoin(user.getId(), client, channel);
                addMessage( client.getNickname() + "(" + client.toString() + ") has joined\r\n", user.getId());
            }
        }
    }
}

void Server::execKICK(Client& client, const std::string line)
{
    std::istringstream iss(line);
    std::string channelName;
    std::string userNick;
    std::string reason;
    const int kickerId(client.getId());
    const std::string kickerNick(client.getNickname());

    iss >> channelName;

    if (channelName[0] != '#')
    {
        replyNoSuchChannel(client);
        return ;
    }

    channelName.erase(0u, 1u);
    ChannelsList::iterator channelIter(getChannel(channelName));

    if(channelIter == _channels.end())
    {
        replyNoSuchChannel(client);
        return ;
    }

    iss >> userNick;
    const ConnectionsList::const_iterator channelUserIter(getClient(userNick));
    std::getline(iss >> std::ws, reason);
    if (reason[0] == ':')
        reason.erase(0u, 1u);
    if (reason.empty())
        reason = kickerNick;

    if (channelUserIter == _connections.end())
    {
        replyNoSuchNick(client, userNick);
        return ;
    }

    const int userId(channelUserIter->getId());

    // if client or channel user are not in channel
    if (!channelIter->isInChannel(userId) || !channelIter->isInChannel(kickerId))
    {
        replyNotInChannel(client, userNick, channelName);
    }
    else if (!channelIter->isOperator(kickerId))
    {
        replyNoPriviledges(client, channelName);
    }
    else // Kick
    {
        const ClientMap& map = channelIter->getClients();
        for (ClientMap::const_iterator it = map.begin(); it != map.end(); it++)
        {
            replyKick(*it->second, client, *channelIter, userNick, reason);
            if (it->second != &(*channelUserIter))
            {
                replyBroadcastKick(it->second->getId(), kickerNick, userNick, channelName, reason);
            }
        }
        channelIter->deleteClient(userId);
        deleteIfEmpty(channelIter);
    }
    // replyYouWereKicked(userId, channelName, kickerNick, reason);
}

void Server::execINVITE(Client& client, const std::string line)
{
    const std::string nickTarget(line.substr(0, line.find('#') - 1));
    const std::string channelTarget(line.substr(line.find('#') + 1));

    ConnectionsList::iterator clientTargetIt = getClient(nickTarget);
    ChannelsList::iterator channelTargetIt = getChannel(channelTarget);

    if (nickTarget.empty() && channelTarget.empty())
    {
        addMessage("Usage: INVITE <nick> [<channel>], invites someone to a channel, by default the current channel (needs chanop)\r\n", client.getId());
    }
    else if (clientTargetIt == _connections.end())
    {
        replyNoSuchNick(client, nickTarget.substr(0, nickTarget.find(' ')));
    }
    else if (channelTargetIt == _channels.end())
    {
        replyNoSuchNick(client, line.substr(line.find('#')));
    }
    else if (!channelTargetIt->isClientInChannel(client.getId()))
    {
        replyNotOnChannelError(client, channelTargetIt->getName());
    }
    else if (channelTargetIt->isClientInChannel(clientTargetIt->getId()))
    {
        replyClientTargetOnChannel(client, clientTargetIt->getNickname(), channelTargetIt->getName());
    }
    else if (!channelTargetIt->isOperator(client.getId()))
    {
        replyChanopNeeded(client, channelTarget, "You must be a channel operator to send an invite.");
    }
    else
    {
        clientTargetIt->setInvited(channelTarget);
        replyInviting(client, nickTarget, channelTargetIt->getName());
        replyInvitingReceived(client, *clientTargetIt, channelTargetIt->getName());
    }
}

void Server::execTOPIC(Client& client, const std::string line)
{
    if (line.empty())
    {
        std::cout << "line empty" << std::endl;
        return ;
    }

    const std::string channelTarget(line.substr(line.find('#') + 1, line.find(' ') - 1));
    const std::string channelTopic(line.substr(line.find(':') + 1));
    
    ChannelsList::iterator channelTargetIt = getChannel(channelTarget);

    if (channelTargetIt == _channels.end())
    {
        if (line.at(0) != '#')
            replyNoSuchChannel(client);
        else
            replyChannelNotFound(client, channelTarget);
    }
    else if (!channelTargetIt->isClientInChannel(client.getId()))
    {
        replyNotOnChannel(client, channelTargetIt->getName());
    }
    else if (line.find(':') != std::string::npos)
    {
        if (channelTargetIt->isTopicRetricted())
        {
            if (!channelTargetIt->isOperator(client.getId()))
            {
                replyNoPriviledges(client, channelTarget);
                return ;
            }
        }
        channelTargetIt->setTopic(channelTopic);
        channelTargetIt->setTopicNick(client.getId());
        replyTopic(client, *channelTargetIt);
    }
    else
    {
        if (channelTargetIt->getTopic() != "")
        {
            std::string clientNickTopic = getClient(channelTargetIt->getTopicNick())->getNickname();
            replyNoTopic(client, *channelTargetIt, clientNickTopic);
        }
        else
        {
            replyNoTopicSet(client, *channelTargetIt);
        }
    }
}

void Server::execPART(Client& client, const std::string line)
{
    const int id(client.getId());
    std::istringstream iss(line);
    std::string channelName;
    std::string reason;

    iss >> channelName;

    if (channelName.empty() || channelName[0] != '#')
    {
        replyPartUsage(id);
        return ;
    }

    channelName.erase(0u, 1u);

    if (!channelExists(channelName))
    {
        replyNoSuchChannel(client);
        replyNoSuchChannelSimple(id, channelName);
    }
    else
    {
        ChannelsList::iterator channelIter(getChannel(channelName));
        std::getline(iss >> std::ws, reason);
        if (reason[0] == ':')
            reason.erase(0u, 1u);

        if (!channelIter->isClientInChannel(id))
        {
            replyNotOnChannel(client, channelName);
        }
        else
        {
            replyYouLeftChannel(id, channelName, reason);

            // reply all channel members
            ClientMap map = channelIter->getClients();
            for (ClientMap::const_iterator it = map.begin(); it != map.end(); it++)
            {
                // send a notice to everyone except for client sending command
                if (it->second != &client)
                    replyBroadcastUserLeft(it->second->getId(), client, reason);
                
                replyPart(client, channelName);
            }
            channelIter->deleteClient(id);
            deleteIfEmpty(channelIter);
        }
    }
}

