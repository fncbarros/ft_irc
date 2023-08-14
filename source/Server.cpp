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
#include "includes/common.hpp"

#include <string.h>
#include <strings.h>
#include <arpa/inet.h>

// Const Definitions
static const char *ADDRESS = "0.0.0.0";

// Special functions
Server::Server(int port, std::string passwd)
: c_operationsPairArray
(
    std::make_pair("JOIN", execJOIN),
    std::make_pair("KICK", execKICK),
    std::make_pair("INVITE", execINVITE),
    std::make_pair("TOPIC", execTOPIC),
    std::make_pair("MODE", execMODE),
    // USER INFO
    std::make_pair("USER", execUSER),
    std::make_pair("PASS", execPASS),
    std::make_pair("NICK", execNICK),
    // Other
    std::make_pair("CAP", execUSER),
)
, c_operationsMap
(
        c_operationsPairArray,
        c_operationsPairArray + sizeof(c_operationsPairArray) / sizeof(c_operationsPairArray[0])
)
, _port(port)
, _socket(0)
, _passwd(passwd)
, _socket_addr()
, _connections()
{
    bzero(&_socket_addr, sizeof(_socket_addr));
    setConnection();
}

Server::~Server()
{
	std::cout << "Closing Server" << std::endl;
    close(_server_socket);
}

// Public functions
void Server::setConnection()
{
    _socket_addr.sin_family = AF_INET;
	_socket_addr.sin_port = htons(_port);
	_socket_addr.sin_addr.s_addr = inet_addr(ADDRESS);

    if ((_server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout << "Failed to create the socket" << std::endl;
        std::cout << "Err: " << strerror(errno) << std::endl;
        // throw err
	}

    if (bind(_server_socket, (s_sockaddr *)&_socket_addr, sizeof(_socket_addr)) < 0)
	{
		std::cout << "Failed to bind the socket" << std::endl;
        std::cout << "Err: " << strerror(errno) << std::endl;
        // throw err
	}

    if (listen(_server_socket, 20) < 0)
    {
        std::cout << "Failed to listen the socket" << std::endl;
        std::cout << "Err: " << strerror(errno) << std::endl;
        // throw err
    }
    
    std::cout << "connection bind" << std::endl;
}

// Internal functions
Server::Server()
{
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
    int new_socket_connection;
	socklen_t sckt_len = sizeof(_socket_addr);
    new_socket_connection = accept(_server_socket, (s_sockaddr *)&_socket_addr, &sckt_len);
    if (new_socket_connection < 0)
    {
        std::cout << "Failed to accept the socket" << std::endl;
        std::cout << "Err: " << strerror(errno) << std::endl;
        // throw err
    }
    else
    {
        _connections.push_back(Client(new_socket_connection));
        FD_SET(new_socket_connection, &_connections_set);
    }
    
    return new_socket_connection;
}

void    Server::inspectEvent(int fd)
{
    std::cout << "inspect Event received" << std::endl;
    for(std::vector<Client>::iterator it = _connections.begin(); it != _connections.end(); it++)
    {
        if (it->getId() == fd)
        {
            parse(readMessage(fd));
        }
        else
        {
            std::cout << "Client not found" << std::endl;
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

    while (true)
    {
        // the fd_set is always destroyed by the select() method
        ready_connections = _connections_set;
        if (select(FD_SETSIZE, &ready_connections, NULL, NULL, NULL) < 0)
        {
            std::cout << "Select error" << std::endl;
            // throw err
        }
        for (int fd = 3; fd < FD_SETSIZE; fd++) 
        {
            if (FD_ISSET(fd, &ready_connections))
            {
                if (fd == _server_socket)
                {
                    std::cout << "new connection created" << std::endl;
                    int new_client = acceptNewConnection();
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

void Server::parse(std::string buffer)
{
    std::istringstream iss(buffer);
    std::string line;
    std::vector<std::string> strList;

    while (std::getline(iss, line))
    {
        strList.push_back(line);
    }

    for (std::vector<std::string>::iterator it = strList.begin(); it != strList.end(); it++)
    {
        line = *it;
        size_t spacePosition = line.find(' ');

        if (spacePosition == std::string::npos || (buffer.find(EOM) + 1) == line.size())
        {
//            break ;
        }

        std::string s1(line.substr(0, spacePosition));
        std::string s2(line.substr(spacePosition + 1));

        OperationsMap::const_iterator map_it = c_operationsMap.find(s1);
        if (map_it != c_operationsMap.end())
        {
            std::cout << map_it->second << ": " << s2 << std::endl;
        }
        else
        {
            std::cout << s1 << s2 << std::endl;
        }
        /*************************TMP*******************************/
        // std::cout << "String1: " << s1 << std::endl;
        // std::cout << "String2: " << s2 << std::endl;
        /*************************TMP*******************************/
    }

    // parse first string
        // if password
        // if USER info
        // if operator
        // check /n/r ??
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
