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

    if (message.first == "JOIN")
        execJOIN(client, message.second);
    else if(message.first == "KICK")
        execKICK(client, message.second);
    else if(message.first == "INVITE")
        execINVITE(client, message.second);
    else if(message.first == "TOPIC")
        execTOPIC(client, message.second);
    else if(message.first == "MODE")
        execMODE(client, message.second);
    else if(message.first == "USER")
        execUSER(client, message.second);
    else if(message.first == "NICK")
        execNICK(client, message.second);
    else if(message.first == "LIST")
        execLIST(client, message.second);
    else if(message.first == "WHO")
        execWHO(client, message.second);
    else if(message.first == "QUIT")
        execQUIT(client, message.second);
    else if(message.first == "PRIVMSG")
        execPRIVMSG(client, message.second);
    else
        std::cout << message.first << " " << message.second << std::endl;
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
    const std::string token = line.substr(0u, line.find(' ')); // TODO: check if find returned npos

    // parse line
    if (token[0] == '#')
    {
        ChannelsList::const_iterator it;
        // lookup channel
        for ( it = _channels.begin(); it != _channels.end(); it++)
        {
            if (token.substr(1u, token.size() - 1) == it->getName())
                break;
        }
        // list users
       it->printList(client.getId());
    }
    else
    {
        // list channels
        replyList(client);
    }
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
            _channels.push_back(Channel(channelName, client));
            replyJoin(client, _channels.back());
        }
        else
        {
            // TODO: need to check if client is not channel user already
            Channel& channel(*channelIt);
            const UserList& list(channel.getList());

            // add user to channel
            channel.addClient(client);
            replyJoin(client, channel);

            // Broadcast to all channel users
            for (UserList::const_iterator userIt = list.begin(); userIt != list.end(); userIt++)
            {
                const Client& user(*(userIt->first));
                //  :pipo!francisco@C82870:FB7641:E1092C:42BAE0:IP JOIN #test * :realname
                Utils::writeTo(":" + client.toString() + " JOIN #" + channel.getName() + " *:realname\r\n", user.getId());
                Utils::writeTo( client.getNickname() + "(" + client.toString() + ") has joined\r\n", user.getId());
            }
        }
    }

}

void Server::execKICK(Client& client, const std::string line)
{
    std::cout << client.getUsername() << ": ";
    std::cout << "***KICK: ";
    std::cout << line << std::endl;
}

void Server::execINVITE(Client& client, const std::string line)
{
    std::cout << client.getUsername() << ": ";
    std::cout << "***INVITE: ";
    std::cout << line << std::endl;
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
