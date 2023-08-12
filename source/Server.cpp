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
#include <iostream>
#include <arpa/inet.h>

Server::Server()
: _port(0)
, _passwd("")
, _socket(0)
, _socket_addr()
{
    bzero(&_socket_addr, sizeof(_socket_addr));
}

Server::Server(int port, std::string passwd)
: _port(port)
, _passwd(passwd)
, _socket(0)
, _socket_addr()
{
    bzero(&_socket_addr, sizeof(_socket_addr));
    setConnection();
}

Server::~Server()
{
	std::cout << "Closing Server" << std::endl;
    close(_socket);
}


void Server::setPort(int port)
{
    _port = port;
}

void Server::setPassword(std::string passwd)
{
    _passwd = passwd;
}

void Server::setConnection()
{
    _socket_addr.sin_family = AF;
	_socket_addr.sin_port = htons(_port);
	_socket_addr.sin_addr.s_addr = inet_addr(ADDRESS);

    if ((_socket = socket(AF, SOCK_STREAM, 0)) < 0)
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

        int socket = accept(_socket, (s_sockaddr *)&_socket_addr, &sckt_len);
        
        if (socket < 0)
        {
            std::cout << "Failed to accept the socket" << std::endl;
            std::cout << "Err: " << strerror(errno) << std::endl;
            break ;
            // throw err
        }
        else
        {
            std::cout << "connection accepted" << std::endl;
            _connections.push_back(Client(socket));
    	    std::cout << "Connection Established" << std::endl;
        }
    }
}