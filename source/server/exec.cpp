/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshintak <bshintak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 19:22:13 by fbarros           #+#    #+#             */
/*   Updated: 2023/09/09 15:07:55 by bshintak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

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
        std::cout << message.first << " " << message.second << std::endl;
    }
}

void Server::execUSER(Client& client, const std::string line)
{
    // parse line
    // ...
    const std::string name = line.substr(0, line.find(' ')); // TODO: check if find returned npos
    if (name.empty())
    {
        // send error
        Utils::writeTo("Missing argument.\n", client.getId());
        return ;
    }

    if (client.getUsername().empty())
    {
        std::cout << "User " << name << " has been added.\n";
    }
    else
    {
        Utils::writeTo("Username " + name + " set.\n", client.getId());
        std::cout << "Username " << name << " set.\n";
    }

    client.setUserActive();
    client.setUsername(name);
    //parse rest??
}

void Server::execNICK(Client& client, const std::string line)
{
    // parse line
    // ...
    const std::string name = line.substr(0, line.find(' ')); // TODO: check if find returned npos
    if (name.empty())
    {
        // send error
        Utils::writeTo("Missing argument.\n", client.getId());
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
    replyWelcome(client);
    client.setNickActive();
    std::cout << "Nickname " << name << " set.\n";
    //parse rest??
}

void Server::execLIST(Client& client, const std::string line)
{
    (void)line;
    replyList(client);
}

void Server::execWHO(Client& client, const std::string line)
{
    // look for channel
    ChannelsList::const_iterator channelIt = getChannel(returnChannelName(line));
    if (channelIt == _channels.end())
    {
        replyWho(client, *channelIt);
        replyEndOfWho(client, *channelIt);
    }
}

void Server::execQUIT(Client& client, const std::string line)
{
    (void)line;
    std::cout << client.getId() << " closed connection.\n";
    deleteClient(client.getId());
}

void Server::execPRIVMSG(Client& client, const std::string line)
{
    std::string token(line.substr(0, line.find(' ')));
    const std::string message(line.substr(line.find(' ') + 1));

    if (token.at(0) == '#')
    {
        token.erase(0, 1); // remove '#'
        ChannelsList::const_iterator channelIt(getChannel(token));
        if (channelIt == _channels.end())
        {
            /// some reply
        }
        else if (channelIt->isInChannel(client.getId()) == false)
        {
            // some other reply
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

/**
 * CHANNEL specific commands
 * */
void Server::execJOIN(Client& client, const std::string line)
{
    std::string channelName(returnChannelName(line));
    if (channelName.empty())
    {
        Utils::writeTo("Usage: JOIN <channel>, joins the channel", client.getId());
    }
    else if (channelName.find(BADJOIN) == 0u)
    {
        replyBadJoin(client, channelName.substr(3u));
    }
    else
    {
        // look for channel
        ChannelsList::iterator channelIt = getChannel(channelName);

        // if no channel, create one
        if (channelIt == _channels.end())
        {
            _channels.push_back(Channel(channelName, &client));
            replyJoin(client, _channels.back());
        }
        else
        {
            // TODO: need to check if client is not channel user already
            Channel& channel(*channelIt);
            const ClientMap& list(channel.getClients());

            // add user to channel
            channel.addClient(client);
            replyJoin(client, channel);

            // Broadcast to all channel users
            for (ClientMap::const_iterator clientIt = list.begin(); clientIt != list.end(); clientIt++)
            {
                const Client& user(*(clientIt->second));
                //  :pipo!francisco@C82870:FB7641:E1092C:42BAE0:IP JOIN #test * :realname
                Utils::writeTo(":" + client.toString() + " JOIN #" + channel.getName() + " *:realname\r\n", user.getId());
                Utils::writeTo( client.getNickname() + "(" + client.toString() + ") has joined\r\n", user.getId());
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

    channelName.erase(0, 1);
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
        reason.erase(0, 1);
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
    }
    replyYouWereKicked(userId, channelName, kickerNick, reason);
}

void Server::execINVITE(Client& client, const std::string line)
{
    const std::string nickTarget(line.substr(0, line.find('#') - 1));
    const std::string channelTarget(line.substr(line.find('#') + 1));

    ConnectionsList::const_iterator clientTargetIt = getClient(nickTarget);
    ChannelsList::iterator channelTargetIt = getChannel(channelTarget);

    //TODO: ERR_NEEDMOREPARAMS (461)
    if (clientTargetIt == _connections.end() || channelTargetIt == _channels.end())
    {
        replyNoSuchNickError(client, nickTarget);
    }
    else if (!channelTargetIt->isClientInChannel(client.getId()))
    {
        replyNotOnChannelError(client, channelTargetIt->getName());
    }
    else if (channelTargetIt->isClientInChannel(clientTargetIt->getId()))
    {
        replyClientTargetOnChannel(client, clientTargetIt->getNickname(), channelTargetIt->getName());
    }
    //TODO: ERR_CHANOPRIVSNEEDED (482): Sent when the user does not have sufficient privileges (channel operator status) to invite users to the specified channel.
    else
    {
        replyInviting(client, nickTarget, channelTargetIt->getName());
        replyInvitingReceived(client, *clientTargetIt, channelTargetIt->getName());
    }
}

void Server::execTOPIC(Client& client, const std::string line)
{
    std::cout << client.getUsername() << ": ";
    std::cout << "***TOPIC: ";
    std::cout << line << std::endl;
}

void Server::execMODE(Client& client, const std::string line)
{
    (void)client;

    std::istringstream iss(line);
    std::string channelName;
    std::string mode;
    std::string params;

    iss >> channelName;

    if (channelName.empty() || mode.empty())
    {
        // reply
        return ;
    }

    // TODO: might need to check for other symbols ('&','+','!')
    if (channelName[0] != '#')
        // reply
    channelName.erase(0, 1);

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

    channelName.erase(0, 1);
    
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
            reason.erase(0, 1);

        if (!channelIter->isClientInChannel(id))
        {
            replyNotOnChannel(client, channelName);
        }
        else
        {
            replyYouLeftChannel(id, channelName, reason);

            // reply all channel memebers
            ClientMap map = channelIter->getClients();
            for (ClientMap::const_iterator it = map.begin(); it != map.end(); it++)
            {
                // send a notice to everyone except for client sending command
                if (it->second != &client)
                    replyBroadcastUserLeft(it->second->getId(), client, reason);
                
                replyPart(client, channelName);
            }
            channelIter->deleteClient(id);
        }
    }
    
}

void Server::execCAP(Client& client, std::string command)
{
    std::string capabilities;
    if (command.find("CAP LS") != std::string::npos)
        capabilities = "CAP * LS :cap1, cap2, cap3";
    else if (command.find("CAP REQ") != std::string::npos)
        capabilities = "CAP * ACK :cap1, cap2, -cap3";
    else if (command.find("CAP END") != std::string::npos)
        capabilities = "CAP * ACK :CAP END";
    replyCAPLS(client, capabilities);
}
