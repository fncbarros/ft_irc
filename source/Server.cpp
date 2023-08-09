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

#include "Server.hpp"

#include <string.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>

Server::Server()
: _port(0)
, _passwd("")
, _socket(0)
, _socket_addr()
{
    memset(&_socket_addr, 0, sizeof(_socket_addr));
}

Server::Server(uint8_t port, std::string passwd)
: _port(port)
, _passwd(passwd)
, _socket(0)
, _socket_addr()
{
    memset(&_socket_addr, 0, sizeof(_socket_addr));
    setConnection();
}

Server::~Server()
{
	std::cout << "Closing connections" << std::endl;

    std::vector<socket_t>::const_iterator it = _connections.cbegin();
    for (; it != _connections.end(); it++)
    {
	    close(*it);
    }
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
    _socket_addr.sin_family = AF_INET;
	_socket_addr.sin_port = htons(_port);
	_socket_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    
    if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		std::cout << "Failed to create the socket" << std::endl;
        // throw err
	}

    if (bind(_socket, (sockaddr *)&_socket_addr, SOCKLEN) < 0)
	{
		std::cout << "Failed to bind the socket" << std::endl;
        // throw err
	}
}

void Server::connectionLoop()
{
	socklen_t sckt_len = SOCKLEN;
    std::vector<socket_t>::iterator it = _connections.begin();

    while (true)
    {
        if (listen(_socket, 20) < 0)
        {
            std::cout << "Failed to listen the socket" << std::endl;
            // throw err
        }

        int it = accept(_socket, (sockaddr *)&_socket_addr, &sckt_len);
        
        if (it < 0)
        {
            std::cout << "Failed to accept the socket" << std::endl;
            // throw err
        }

    	std::cout << "Connection Established" << std::endl;
    }
}