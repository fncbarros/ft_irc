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

#include <string.h>
#include <strings.h>
#include <arpa/inet.h>
#include <cctype>

// Special functions
Server::Server()
: _passwd()
, _socket_addr()
, _connections()
, _interrupt(false)
{
    bzero(&_socket_addr, sizeof(_socket_addr));
}

Server::~Server()
{
    _interrupt = true;
	std::cout << "Closing Server" << std::endl;
    if (!_connections.empty())
    {
        for (std::vector<Client *>::iterator it = _connections.begin(); it != _connections.end(); it++)
        {
            if (*it)
                delete *it;
        }
    }
    if (_server_socket)
        close(_server_socket);
}

// Public functions
int Server::setConnection(const int port, const std::string password)
{
    (void) password;
    _socket_addr.sin_family = AF_INET;
	_socket_addr.sin_port = htons(port);
	_socket_addr.sin_addr.s_addr = inet_addr("0.0.0.0");

    if ((_server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout << "Failed to create the socket" << std::endl;
        std::cout << "Err: " << strerror(errno) << std::endl;
        return (1);
	}

    if (bind(_server_socket, (sockaddr *)&_socket_addr, sizeof(_socket_addr)) < 0)
	{
		std::cout << "Failed to bind the socket" << std::endl;
        std::cout << "Err: " << strerror(errno) << std::endl;
        return (1);
	}

    if (listen(_server_socket, 20) < 0)
    {
        std::cout << "Failed to listen the socket" << std::endl;
        std::cout << "Err: " << strerror(errno) << std::endl;
        return (1);
    }
    
    std::cout << "connection bind" << std::endl;
    return (0);
}

void Server::interrupt()
{
    _interrupt = true;
}

std::string    Server::readMessage(int fd) const
{
    std::cout << "connection accepted" << std::endl;
    char buffer[BUFFER_SIZE] = {0};

    int bytesReceived = recv(fd, buffer, BUFFER_SIZE, 0);
    if (bytesReceived < 0)
    {
        std::cout << "Failed to read Client Socket" << std::endl;
    }
    std::cout << "Client message received" << std::endl;
    return (buffer);
}

int Server::acceptNewConnection()
{
    if (_interrupt)
        return (-1);
    int new_socket_connection;
	socklen_t sckt_len = sizeof(_socket_addr);
    new_socket_connection = accept(_server_socket, (sockaddr *)&_socket_addr, &sckt_len);
    if (new_socket_connection < 0)
    {
        std::cout << "Failed to create the connection" << std::endl;
        std::cout << "Err: " << strerror(errno) << std::endl;
        return (-1);
    }
    else
    {
        _connections.push_back(new Client(new_socket_connection));
        FD_SET(new_socket_connection, &_connections_set);
        std::cout << "new connection created" << std::endl;
    }

    return new_socket_connection;
}

void    Server::inspectEvent(int fd)
{
    if (!_interrupt)
    {
        return ;
    }

    const std::string rawMsg = readMessage(fd);
    const tokenList processedMsg = parse(rawMsg);
    Client *client;

    for(std::vector<Client *>::iterator it = _connections.begin(); it != _connections.end(); it++)
    {
        if ((*it)->getId() == fd)
        {
            client = *it;
        }

    }

    for (tokenList::const_iterator line = processedMsg.begin(); line != processedMsg.end(); line++)
    {
        if (line->first == "JOIN")
        {
            client->execJOIN(line->second);
        }
        else if(line->first == "KICK")
        {
            client->execKICK(line->second);
        }
        else if(line->first == "INVITE")
        {
            client->execINVITE(line->second);
        }        
        else if(line->first == "TOPIC")
        {
            client->execTOPIC(line->second);
        }
        else if(line->first == "MODE")
        {
            client->execMODE(line->second);
        }
        else if(line->first == "USER")
        {
            client->execUSER(line->second);
        }
        else if(line->first == "PASS")
        {
            client->execPASS(line->second);
        }
        else if(line->first == "NICK")
        {
            client->execNICK(line->second);
        }
        else if(line->first == "LIST")
        {
            client->execLIST();
        }
        else if(line->first == "WHO")
        {
            client->execWHO();
        }
        else if(line->first == "QUIT")
        {
            client->execQUIT();
        }
        else if(line->first == "PRIVMSG")
        {
            client->execPRIVMSG(line->second);
        }
        else
        {
            std::cout << line->first << line->second << std::endl;
        }
    }
}

void Server::connectionLoop()
{
    fd_set ready_connections;
    // initialize the fd_set
    FD_ZERO(&_connections_set);
    FD_SET(_server_socket, &_connections_set);

    while (!_interrupt)
    {
        // the fd_set is always destroyed by the select() method
        ready_connections = _connections_set;
        if (select(FD_SETSIZE, &ready_connections, NULL, NULL, NULL) < 0)
        {
            std::cout << "Select error" << std::endl;
            continue;
        }
        for (int fd = 3; fd < FD_SETSIZE; fd++)
        {
            if (FD_ISSET(fd, &ready_connections))
            {
                if (fd == _server_socket)
                {
                    if (int new_client = acceptNewConnection() > 0)
                        inspectEvent(new_client);
                }
                else
                {
                    inspectEvent(fd);
                }
            }
        }
    }
}

tokenList Server::parse(std::string buffer)
{
    std::istringstream iss(buffer);
    std::string line;
    std::vector<std::string> strList;
    tokenList list;   

    while (std::getline(iss, line))
    {
        strList.push_back(line);
    }

    for (std::vector<std::string>::iterator it = strList.begin(); it != strList.end(); it++)
    {
        line = *it;
        size_t spacePosition = line.find(' ');

        if (spacePosition == std::string::npos)
        {
           continue ;
        }

        std::string s1(line.substr(0, spacePosition));
        std::string s2(line.substr(spacePosition + 1));

        validateToken(s1);

        list.push_back(tokenPair(s1, s2));
    }

    return list;
}

void Server::validateToken(std::string& token) const
{
    size_t i;

    std::string tmp = Utils::toUpper(token);
    for (i = 0; i < token_num; i++)
    {
        if (possible_tokens[i] == tmp)
        {
            tmp = possible_tokens[i];
            break ;
        }
    }
    if (i < token_num)
    {
        token = tmp;
    }
}
