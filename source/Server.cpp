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
	_socket_addr.sin_addr.s_addr = inet_addr(ADDRESS);

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
        for(std::vector<Client *>::iterator it = _connections.begin(); it != _connections.end(); it++)
        {
            if ((*it)->getId() == fd)
            {
                tokenList msg = parse(readMessage(fd));
                exec(msg);
            }
        }
    }
}

void Server::connectionLoop()
{
    fd_set ready_connections;
    int actual_max_socket = 0;
    // initialize the fd_set
    FD_ZERO(&_connections_set);
    FD_SET(_server_socket, &_connections_set);
    actual_max_socket = _server_socket;

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

std::string toUpper(const std::string& original)
{
    std::string result = original;
    for (size_t i = 0; i < original.length(); i++)
    {
        result[i] = std::toupper(original[i]);
    }
    return result;
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
    token = (i < token_num) ? tmp : "IGNORE";
}

void Server::exec(tokenList map)
{
    for (tokenList::iterator it = map.begin(); it != map.end(); it++)
    {
        if (it->first == "JOIN")
        {
            execJOIN(it->second);
        }
        else if(it->first == "KICK")
        {
            execKICK(it->second);
        }
        else if(it->first == "INVITE")
        {
            execINVITE(it->second);
        }        
        else if(it->first == "TOPIC")
        {
            execTOPIC(it->second);
        }
        else if(it->first == "MODE")
        {
            execMODE(it->second);
        }
        else if(it->first == "USER")
        {
            execUSER(it->second);
        }
        else if(it->first == "PASS")
        {
            execPASS(it->second);
        }
        else if(it->first == "NICK")
        {
            execNICK(it->second);
        }
        else
        {
            std::cout << "token not recognised\n";
        }
    }
}

void Server::execJOIN(const std::string line)
{
    std::cout << "***JOIN: ";
    std::cout << line << std::endl;
}

void Server::execKICK(const std::string line)
{
    std::cout << "***KICK: ";
    std::cout << line << std::endl;
}

void Server::execINVITE(const std::string line)
{
    std::cout << "***INVITE: ";
    std::cout << line << std::endl;
}

void Server::execTOPIC(const std::string line)
{
    std::cout << "***TOPIC: ";
    std::cout << line << std::endl;
}

void Server::execMODE(const std::string line)
{
    std::cout << "***MODE: ";
    std::cout << line << std::endl;
}

void Server::execUSER(const std::string line)
{
    std::cout << "***USER: ";
    std::cout << line << std::endl;
}

void Server::execPASS(const std::string line)
{
    std::cout << "***PASS: ";
    std::cout << line << std::endl;
}

void Server::execNICK(const std::string line)
{
    std::cout << "***NICK: ";
    std::cout << line << std::endl;
}
