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
, _socket_process(0)
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
    close(_socket);
}

// Public functions
void Server::setConnection()
{
    _socket_addr.sin_family = AF_INET;
	_socket_addr.sin_port = htons(_port);
	_socket_addr.sin_addr.s_addr = inet_addr(ADDRESS);

    if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout << "Failed to create the socket" << std::endl;
        std::cout << "Err: " << strerror(errno) << std::endl;
        // throw err
	}

    if (bind(_socket, (sockaddr *)&_socket_addr, sizeof(_socket_addr)) < 0)
	{
		std::cout << "Failed to bind the socket" << std::endl;
        std::cout << "Err: " << strerror(errno) << std::endl;
        // throw err
	}
    std::cout << "connection bind" << std::endl;
}

// Internal functions
Server::Server()
{
}

std::string    Server::readMessage(void) const
{
    char buffer[BUFFER_SIZE] = {0};

    int bytesReceived = recv(_socket_process, buffer, BUFFER_SIZE, 0);
    if (bytesReceived < 0)
    {
        std::cout << "Failed to read Client Socket" << std::endl;
    }
    /*************************TMP*******************************/
    std::cout << "Client message received" << std::endl;
    // std::cout << "[ " << buffer << " ]" << std::endl;
    /*************************TMP*******************************/
    return (buffer);
}

void Server::connectionLoop()
{
	socklen_t sckt_len = SOCKLEN;
    
    if (errno)
    {
        return ;
    }

    while (true)
    {
        if (listen(_socket, 20) < 0)
        {
            std::cout << "Failed to listen the socket" << std::endl;
            std::cout << "Err: " << strerror(errno) << std::endl;
            break ;
            // throw err
        }

        _socket_process = accept(_socket, (sockaddr *)&_socket_addr, &sckt_len);
        
        if (_socket_process < 0)
        {
            std::cout << "Failed to accept the socket" << std::endl;
            std::cout << "Err: " << strerror(errno) << std::endl;
            break ;
            // throw err
        }
        else
        {
            std::cout << "connection accepted" << std::endl;
            parse(readMessage());
            std::cout << "Connection Established" << std::endl;
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
