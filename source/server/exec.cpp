/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbarros <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 19:22:13 by fbarros           #+#    #+#             */
/*   Updated: 2023/08/09 19:22:16 by fbarros          ###   ########.fr       */
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
    }
    std::cout << client.getUsername() << ": ";
    (void)line;
    std::cout << "***LIST***\n";
}

void Server::execWHO(Client& client, const std::string line)
{
    size_t pos(line.find('#'));
    if (pos == std::string::npos)
        std::cerr << "Error: join: could not find '#' token." << std::endl; // TODO: reply error
    const std::string channelName(line.substr(pos + 1));

    // look for channel
    ChannelsList::const_iterator channelIt = getChannel(channelName);
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

    ConnectionsList::iterator newClient = getClient(nickname);

    if (newClient == _connections.end())
        replyPrivMessageNickNotFound(client, nickname);
    else
    {
       replyPrivateMessage(client, *newClient, messageReceived);
    }

    std::cout << "Private Message " << client.getNickname() << " user: [" << nickname << "] message: [" << messageReceived << "]" << std::endl;
}

/**
 * CHANNEL specific commands
 * */

void Server::execJOIN(Client& client, const std::string line)
{
    // TMP
    std::cout << client.getUsername() << ": ";
    std::cout << line << std::endl;
    // TMP

    size_t pos(line.find('#'));
    if (pos == std::string::npos)
        std::cerr << "Error: join: could not find '#' token." << std::endl; // TODO: reply error
    const std::string channelName(line.substr(pos + 1));

    // look for channel
    ChannelsList::const_iterator it = getChannel(channelName);
    
    // if no channel, create one
    if (it == _channels.end())
    {
        _channels.push_back(Channel(channelName, client));
        const Channel channel(_channels.back());
        replyJoin(client, channel);
    }
    else
    {
        // add user to channel
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
    size_t pos(line.find('#'));
    if (pos == std::string::npos)
        std::cerr << "Error: join: could not find '#' token." << std::endl; // TODO: reply error
    const std::string channelName(line.substr(pos + 1));

    // look for channel
    ChannelsList::const_iterator channelIt = getChannel(channelName);

    if (channelIt != _channels.end())
        replyChannelMode(client, *channelIt);

    replyName(client, *channelIt);
    replyEndOfNames(client, *channelIt);
    replyChannelMode(client, *channelIt);
    replyCreationTime(client, *channelIt);
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
