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


void Server::execJOIN(Client& client, const std::string line)
{
    std::cout << client.getUsername() << ": ";
    std::cout << "***JOIN: ";
    std::cout << line << std::endl;
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
    std::cout << client.getUsername() << ": ";
    std::cout << "***MODE: ";
    std::cout << line << std::endl;
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

    if (client.getNickname().empty())
    {
        std::cout << "Nick " << name << " has been added.\n";
    }
    else
    {
        Utils::writeTo("Nickname " + name + " set.\n", client.getId());
        std::cout << "Nickname " << name << " set.\n";

    }

        client.setNickActive();
        client.setNickname(name);
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
        printList(it->getList(), client.getId()); // TODO: need getter for Channel
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
    std::cout << client.getUsername() << ": ";
    (void)line;
    std::cout << "***WHO***\n";
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
