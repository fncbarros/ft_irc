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
        else if(line->first == "PASS")
            execPASS(client, line->second);
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
    (void)client;
    std::cout << "***USER: ";
    std::cout << line << std::endl;
}

void Server::execPASS(Client& client, const std::string line)
{
    std::cout << client.getUsername() << ": ";
    std::cout << "***PASS: ";
    std::cout << line << std::endl;
}

void Server::execNICK(Client& client, const std::string line)
{
    std::cout << client.getUsername() << ": ";
    std::cout << "***NICK: ";
    std::cout << line << std::endl;
}

void Server::execLIST(Client& client, const std::string line)
{
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
