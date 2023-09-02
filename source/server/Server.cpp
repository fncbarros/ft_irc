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

#include "../includes/Server.hpp"
#include <strings.h>


// Special functions
Server::Server()
: _server_socket(0)
, _interrupt(false)
{
    bzero(&_socket_addr, sizeof(_socket_addr));
}

Server::~Server()
{
    _interrupt = true;
	std::cout << "Closing Server" << std::endl;
    for (ConnectionsList::iterator client = _connections.begin(); client != _connections.end(); client++)
    {
        if (client->getId() != 0)
            close(client->getId());
    }
    if (_server_socket)
        close(_server_socket);
}

// Public functions

void Server::interrupt()
{
    _interrupt = true;
}

void Server::setPassword(const std::string password)
{
    _password = password;
}

int Server::acceptNewConnection()
{
    if (_interrupt)
        return -1;

    int new_socket_connection;
	socklen_t sckt_len = sizeof(_socket_addr);
    new_socket_connection = accept(_server_socket, (sockaddr *)&_socket_addr, &sckt_len);

    if (new_socket_connection < 0)
    {
        std::cout << "Failed to create the connection" << std::endl;
        std::cout << "Err: " << strerror(errno) << std::endl;
        return -1;
    }
    else
    {
        _connections.push_back(Client(new_socket_connection));
        FD_SET(new_socket_connection, &_connections_set);
        std::cout << "new connection created" << std::endl;
    }

    return new_socket_connection;
}

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
        return 1;
	}

    if (bind(_server_socket, (sockaddr *)&_socket_addr, sizeof(_socket_addr)) < 0)
	{
		std::cout << "Failed to bind the socket" << std::endl;
        std::cout << "Err: " << strerror(errno) << std::endl;
        return 1;
	}

    if (listen(_server_socket, 20) < 0)
    {
        std::cout << "Failed to listen the socket" << std::endl;
        std::cout << "Err: " << strerror(errno) << std::endl;
        return 1;
    }
    
    std::cout << "connection bind" << std::endl;
    return 0;
}

bool Server::inspectEvent(int fd)
{
    bool ret = true;
    if (_interrupt)
        return true;

    // if new connection
    if (fd == _server_socket)
    {
        // if connection failed, return
        if ((fd = acceptNewConnection()) < 0)
            return true;
    }

    const std::string rawMsg = readMessage(fd);
    if (rawMsg.empty())
        return false;
    tokenList processedMsg = parse(rawMsg);
    ConnectionsList::iterator client = getClient(fd);

    if (client != _connections.end())
    {
        for (tokenList::iterator message = processedMsg.begin(); message != processedMsg.end(); message++)
        {
            std::cout << "message: " << message->first << " " << message->second << std::endl;
            if (message != processedMsg.end())
            {
                if (!message->first.compare("CAP"))
                {
                    std::cout << "command was cap ls" << std::endl;
                }
                else if (!client->isValid())
                {
                    if (auth(*client, *message) == false)
                    {
                        ret = false;
                        break;
                    }
                }
                else
                    exec(*client, processedMsg);
            }
        }
    } 
        
    return ret;
}


void Server::connectionLoop()
{
    fd_set ready_connections;
    // timeval time;
    // time.tv_sec = 0;
    // time.tv_usec = 500;
    // initialize the fd_set
    FD_ZERO(&_connections_set);
    FD_SET(_server_socket, &_connections_set);

    while (!_interrupt)
    {
        // the fd_set is always destroyed by the select() method
        ready_connections = _connections_set;
        int ret = select(FD_SETSIZE, &ready_connections, NULL, NULL, NULL);
        if (ret < 0)
        {
            std::cout << "Select error: " << strerror(errno) << std::endl;
            continue;
        }
        else if (ret == 0)
            continue ;
        for (int fd = 3; fd < FD_SETSIZE; fd++)
        {
            // if fd is not ready for reading, go to next one
            if (!FD_ISSET(fd, &ready_connections))
                continue ;
            if (!inspectEvent(fd)) // Note: inspectEvent() validates clientFd
            {
                deleteClient(fd);
                ready_connections = _connections_set;
            }
        }
    }
}

