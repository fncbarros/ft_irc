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
: _password()
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
    for (std::vector<Client>::iterator client = _connections.begin(); client != _connections.end(); client++)
    {
        if (client->getId() != 0)
            close(client->getId());
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

void Server::interrupt()
{
    _interrupt = true;
}

std::string    Server::readMessage(int fd) const
{
    std::cout << "connection accepted" << std::endl;
    char buffer[BUFFER_SIZE] = {0};
    bzero(buffer, BUFFER_SIZE);

    int bytesReceived = recv(fd, buffer, BUFFER_SIZE, 0);

    if (bytesReceived < 0)
        std::cout << "Failed to read Client Socket" << std::endl;

    std::cout << "Client message received" << std::endl;
    return buffer;
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

bool Server::inspectEvent(int fd)
{
    if (_interrupt)
        return true;

    if (fd == _server_socket)
    {
        // if connection failed, return
        if ((fd = acceptNewConnection()) < 0)
            return true;
    }

    const std::string rawMsg = readMessage(fd);
    if (rawMsg.empty())
        return false;

    const tokenList processedMsg = parse(rawMsg);
    std::vector<Client>::iterator client = getClient(fd);
    if (client != _connections.end())
        exec(*client, processedMsg);
    return true;
}

void Server::exec(Client& client, tokenList processedMsg)
{
    for (tokenList::const_iterator line = processedMsg.begin(); line != processedMsg.end(); line++)
    {
        if (line->first == "JOIN")
            execJOIN(client, line->second);
        else if(line->first == "KICK")
            execKICK(client, line->second);
        else if(line->first == "INVITE")
            execINVITE(client, line->second);
        else if(line->first == "TOPIC")
            execTOPIC(client, line->second);
        else if(line->first == "MODE")
            execMODE(client, line->second);
        else if(line->first == "USER")
            execUSER(client, line->second);
        else if(line->first == "PASS")
            execPASS(client, line->second);
        else if(line->first == "NICK")
            execNICK(client, line->second);
        else if(line->first == "LIST")
            execLIST(client, line->second);
        else if(line->first == "WHO")
            execWHO(client, line->second);
        else if(line->first == "QUIT")
            execQUIT(client, line->second);
        else if(line->first == "PRIVMSG")
            execPRIVMSG(client, line->second);
        else
            std::cout << line->first << line->second << std::endl;
    }
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

tokenList Server::parse(std::string buffer)
{
    std::istringstream iss(buffer);
    std::string line;
    std::vector<std::string> strList;
    tokenList list;   

    while (std::getline(iss, line))
        strList.push_back(line);

    for (std::vector<std::string>::iterator it = strList.begin(); it != strList.end(); it++)
    {
        line = *it;
        size_t spacePosition = line.find(' ');

        if (spacePosition == std::string::npos)
           continue ;

        std::string s1(line.substr(0, spacePosition));
        std::string s2(line.substr(spacePosition + 1));

        validateToken(s1);

        list.push_back(tokenPair(s1, s2));
    }

    return list;
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
    if (i < token_num)
        token = tmp;
}

bool Server::auth( const std::string &password) const
{
    return (password == _password);
}

std::vector<Client>::iterator Server::getClient(const int fd)
{
    for (std::vector<Client>::iterator client = _connections.begin(); client != _connections.end(); client++)
    {
        if (client->getId() == fd)
            return client;
    }
    return _connections.end();
}

void Server::deleteClient(const int fd)
{
    FD_CLR(fd, &_connections_set);

    std::vector<Client>::iterator client = getClient(fd);
    if (client != _connections.end())
        _connections.erase(client);
    close(fd);
}

void Server::execJOIN(Client& client, const std::string line)
{
    std::cout << client.getUsername() << ": ";
    std::cout << "***JOIN: ";
    std::cout << line << std::endl;
}

void Server::execKICK(Client& client, const std::string line)
{
    std::cout << client.getUsername() << ": ";
    std::cout << "***KICK: ";
    std::cout << line << std::endl;
}

void Server::execINVITE(Client& client, const std::string line)
{
    std::cout << client.getUsername() << ": ";
    std::cout << "***INVITE: ";
    std::cout << line << std::endl;
}

void Server::execTOPIC(Client& client, const std::string line)
{
    std::cout << client.getUsername() << ": ";
    std::cout << "***TOPIC: ";
    std::cout << line << std::endl;
}

void Server::execMODE(Client& client, const std::string line)
{
    std::cout << client.getUsername() << ": ";
    std::cout << "***MODE: ";
    std::cout << line << std::endl;
}

void Server::execUSER(Client& client, const std::string line)
{
    (void)client;
    std::cout << "***USER: ";
    std::cout << line << std::endl;
}

void Server::execPASS(Client& client, const std::string line)
{
    std::cout << client.getUsername() << ": ";
    std::cout << "***PASS: ";
    std::cout << line << std::endl;
}

void Server::execNICK(Client& client, const std::string line)
{
    std::cout << client.getUsername() << ": ";
    std::cout << "***NICK: ";
    std::cout << line << std::endl;
}

void Server::execLIST(Client& client, const std::string line)
{
    std::cout << client.getUsername() << ": ";
    (void)line;
    std::cout << "***LIST***\n";
}

void Server::execWHO(Client& client, const std::string line)
{
    std::cout << client.getUsername() << ": ";
    (void)line;
    std::cout << "***WHO***\n";
}

void Server::execQUIT(Client& client, const std::string line)
{
    (void)line;

    std::cout << client.getUsername() << " closed connection.\n";
}

void Server::execPRIVMSG(Client& client, const std::string line)
{
    std::cout << client.getUsername() << ": ";
    std::cout << "***PRIVMSG: ";
    std::cout << line << std::endl;
}
