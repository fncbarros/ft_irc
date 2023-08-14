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
: _port(port)
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

    while (std::getline(iss, line))
    {
        size_t spacePosition = line.find(' ');

        if (spacePosition == std::string::npos || (buffer.find(EOM) + 1) == line.size())
        {
            break ;
        }

        std::string s1(line.substr(0, spacePosition));
        std::string s2(line.substr(spacePosition + 1));

        OperationsMap::const_iterator it = c_operationsMap.find(s1);
        if (it != c_operationsMap.end())
        {
            std::cout << it->second << ": " << s2 << std::endl;
        }
        else
        {
            std::cout << s1 << std::endl;
        }
        /*************************TMP*******************************/
//        std::cout << "String1: " << s1 << std::endl;
//        std::cout << "String2: " << s2 << std::endl;
        /*************************TMP*******************************/
    }

    // parse first string
        // if password
        // if USER info
        // if operator
        // check /n/r ??
}