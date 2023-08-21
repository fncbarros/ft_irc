/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbarros <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 19:22:13 by fbarros           #+#    #+#             */
/*   Updated: 2023/08/09 19:22:16 by fbarros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Server.hpp"

void Server::exec(Client& client, tokenList processedMsg)
{
    for (tokenList::const_iterator line = processedMsg.begin(); line != processedMsg.end(); line++)
    {
        if (line->first == "JOIN")
            execJOIN(client, line->second);
        else if(line->first == "KICK")
            execKICK(client, line->second);
        else if(line->first == "INVITE")
            execINVITE(client, line->second);
        else if(line->first == "TOPIC")
            execTOPIC(client, line->second);
        else if(line->first == "MODE")
            execMODE(client, line->second);
        else if(line->first == "USER")
            execUSER(client, line->second);
        else if(line->first == "NICK")
            execNICK(client, line->second);
        else if(line->first == "LIST")
            execLIST(client, line->second);
        else if(line->first == "WHO")
            execWHO(client, line->second);
        else if(line->first == "QUIT")
            execQUIT(client, line->second);
        else if(line->first == "PRIVMSG")
            execPRIVMSG(client, line->second);
        else
            std::cout << line->first << line->second << std::endl;
    }
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
    const std::string name = line.substr(0, line.find(' '));
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
            Utils::writeTo("User " + name + " already in use.\n", client.getId());
            return ;
        }
    }

    if (client.getUsername().empty())
    {
        Utils::writeTo("User " + name + " has been added.\n", client.getId());
        std::cout << "User " << name << " has been added.\n";
    }
    else
    {
        Utils::writeTo("Username " + name + " set.\n", client.getId());
        std::cout << "Username " << name << " set.\n";
    }

    client.setUsername(name);
    //parse rest??
}

void Server::execNICK(Client& client, const std::string line)
{
    // parse line
    // ...
    const std::string name = line.substr(0, line.find(' '));
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
            Utils::writeTo("Nick " + name + " already in use.\n", client.getId());
            return ;
        }
    }

    if (client.getNickname().empty())
    {
        Utils::writeTo("Nick " + name + " has been added.\n", client.getId());
        std::cout << "Nick " << name << " has been added.\n";
    }
    else
    {
        Utils::writeTo("Nickname " + name + " set.\n", client.getId());
        std::cout << "Nickname " << name << " set.\n";
    }

    client.setNickname(name);
    //parse rest??
}

void Server::execLIST(Client& client, const std::string line)
{
    // parse line
    if (line[0] == '#')
    {
        // lookup channel
        // list users
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
    std::cout << client.getUsername() << " closed connection.\n";
    deleteClient(client.getId());
}

void Server::execPRIVMSG(Client& client, const std::string line)
{
    std::cout << client.getUsername() << ": ";
    std::cout << "***PRIVMSG: ";
    std::cout << line << std::endl;
}
