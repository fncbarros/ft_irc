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

// Const Definitions
static const char *ADDRESS = "0.0.0.0";

// Special functions
Server::Server(int port, std::string passwd)
: _port(port)
, _server_socket(0)
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
    std::cout << "[ " << buffer << " ]" << std::endl;
    std::cout << "Connection Established" << std::endl;
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
            std::cout << readMessage(it->getId()) << std::endl;
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
