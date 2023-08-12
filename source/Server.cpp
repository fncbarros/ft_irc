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

{
}

Server::Server(int port, std::string passwd)
: _port(port)
, _socket(0)
, _socket_process()
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

void    Server::read_message(void) const
{
    char buffer[BUFFER_SIZE] = {0};

    int bytesReceived = recv(_socket_process, buffer, BUFFER_SIZE, 0);
    if (bytesReceived < 0)
    {
        std::cout << "Failed to read Client Socket" << std::endl;
    }
    std::cout << "Client message received" << std::endl;
    std::cout << "[ " << buffer << " ]" << std::endl;
}

// void    Server::write_message()
// {
//     std::string buffer(":Welcome to the Internet Relay Network "+ _nickname + "!" + _username + "@" + ADDRESS + "\n");
//     const size_t fd = send(_socket_process, buffer.c_str(), buffer.size(), 0);
//     if (fd == buffer.size())
//     {
//         std::cout << "Message sent" << std::endl;
//     }
//     else
//     {
//         std::cout << "Message fail :/" << std::endl;
//     }
// }

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
            read_message();
    	    std::cout << "Connection Established" << std::endl;
        }
    }
}