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
#include <iostream>
#include <arpa/inet.h>
#include <sstream>

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

    if (bind(_socket, (s_sockaddr *)&_socket_addr, sizeof(_socket_addr)) < 0)
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

        _socket_process = accept(_socket, (s_sockaddr *)&_socket_addr, &sckt_len);
        
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
            parse();
            std::cout << "Connection Established" << std::endl;
        }
    }
}

void Server::parse()
{
    std::string buffer(readMessage());
    size_t eol = 0u;

    while (buffer.substr(eol, 3u) != EOM)
    {
        size_t point = buffer.find(" ");
        eol = buffer.find("\n");
        std::string s1(buffer.substr(0, point));
        std::string s2(buffer.substr(point + 1, eol));
        /*************************TMP*******************************/
        std::cout << "String1: " << s1 << std::endl;
        std::cout << "String2: " << s2 << std::endl;
        /*************************TMP*******************************/
    }

    // parse first string
        // if password
        // if USER info
        // if operator
        // check /n/r ??
}