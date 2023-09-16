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
    const std::string nickname(line.substr(0, line.find(' ')));
    const std::string messageReceived(line.substr(line.find(' ') + 1));

    if (nickname.at(0) == '#')
    {
        channelPrivateMessage(client, nickname.substr(1), messageReceived);
    }
    else
    {
        clientPrivateMessage(client, nickname, messageReceived);
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
            const ClientMap list(channel.getClients());

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
    const size_t hashPos(line.find('#'));
    const std::string channelName((hashPos != std::string::npos) ? line.substr(hashPos + 1, line.find(" ") - 1) : "");
    const std::string userNick(line.substr(line.find_first_not_of(" ", hashPos + channelName.size() + 1) , line.find(" ")));
    const std::string reason(line.substr(line.find_first_not_of(" ", hashPos + channelName.size() + 1 + userNick.size())));
    const ConnectionsList::const_iterator clientIter(getClient(userNick));
    ChannelsList::iterator channelIter(getChannel(channelName));

    if (channelIter == _channels.end())
    {
        replyNoSuchChannel(client);
    }
    else if (clientIter == _connections.end())
    {
        replyNoSuchNick(client, userNick);
    }
    else
    {
        const int kickerId(client.getId());
        const int userId(clientIter->getId());

        // if user is not in channel
        if (!channelIter->isInChannel(userId))
        {
            replyNotInChannel(client, userNick, channelName);
        }
        else if (!channelIter->isOperator(kickerId))
        {
            const std::string reply("#" + channelName + " :You're not channel operator\r\n");
            Utils::writeTo(reply, client.getId());
            replyNoPriviledges(client, reply);
        }
        else
        {
            replyKick(client, *channelIter, userNick, reason);
            channelIter->deleteClient(userId);
            Utils::writeTo("You were kicked from #" + channelName + " by " + client.getNickname() + "(" + reason + ")\r\n", userId);
        }
    }
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
    std::cout << client.getId() << " " + line << std::endl;
}

void Server::execPART(Client& client, const std::string line)
{
    // input: "PART #<channel> :Leaving"
    // out:
    // to sending client: "You have left channel #<channel> (Leaving)

    std::cout << client.getUsername() << ": ";
    std::cout << "***PART: ";
    std::cout << "Client " << client.getNickname() << " left channel " << line << std::endl;
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
