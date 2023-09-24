/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshintak <bshintak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 19:22:13 by fbarros           #+#    #+#             */
/*   Updated: 2023/09/09 15:16:35 by bshintak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

// Special functions
Server::Server()
: _server_socket(0)
, _interrupt(false)
{
    bzero(&_socket_addr, sizeof(_socket_addr));
    _commands.insert(std::make_pair<std::string, exec_ptr>("JOIN",&Server::execJOIN));
    _commands.insert(std::make_pair<std::string, exec_ptr>("KICK",&Server::execKICK));
    _commands.insert(std::make_pair<std::string, exec_ptr>("INVITE",&Server::execINVITE));
    _commands.insert(std::make_pair<std::string, exec_ptr>("TOPIC",&Server::execTOPIC));
    _commands.insert(std::make_pair<std::string, exec_ptr>("MODE",&Server::execMODE));
    _commands.insert(std::make_pair<std::string, exec_ptr>("USER",&Server::execUSER));
    _commands.insert(std::make_pair<std::string, exec_ptr>("NICK",&Server::execNICK));
    _commands.insert(std::make_pair<std::string, exec_ptr>("LIST",&Server::execLIST));
    _commands.insert(std::make_pair<std::string, exec_ptr>("WHO",&Server::execWHO));
    _commands.insert(std::make_pair<std::string, exec_ptr>("QUIT",&Server::execQUIT));
    _commands.insert(std::make_pair<std::string, exec_ptr>("PRIVMSG",&Server::execPRIVMSG));
    _commands.insert(std::make_pair<std::string, exec_ptr>("PART",&Server::execPART));
    
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

void    Server::interrupt()
{
    _interrupt = true;
}

void    Server::setPassword(const std::string password)
{
    _password = password;
}

void    Server::setCurrentDate(void)
{
    time_t now = time(0);
    _server_date_created = ctime(&now);
    if (!_server_date_created.empty())
        _server_date_created.resize(_server_date_created.size() - 1);
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
        fcntl(new_socket_connection, F_SETFL, O_NONBLOCK);
        _connections.push_back(Client(new_socket_connection));
        FD_SET(new_socket_connection, &_connections_set_read);
        FD_SET(new_socket_connection, &_connections_set_write);
        std::cout << "new connection created" << std::endl;
    }

    return new_socket_connection;
}

int Server::setConnection(const int port, const std::string password)
{
    setPassword(password);
    _socket_addr.sin_family = AF_INET;
	_socket_addr.sin_port = htons(port);
	_socket_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    setCurrentDate();

    if ((_server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout << "Failed to create the socket" << std::endl;
        std::cout << "Err: " << strerror(errno) << std::endl;
        return 1;

	}
    fcntl(_server_socket, F_SETFL, O_NONBLOCK);

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

    std::string rawMsg = readMessage(fd);
    tokenList processedMsg = parse(rawMsg);
    ConnectionsList::iterator client = getClient(fd);

    if (client == _connections.end())
        return false;

    for (tokenList::iterator message = processedMsg.begin(); message != processedMsg.end(); message++)
    {
        std::cout << "message: " << message->first << " " << message->second << std::endl;
        if (message->first.empty())
        {
            deleteClient(client->getId());
        }
        if (!message->first.compare("CAP"))
        {
            execCAP(*client, rawMsg);
        }
        else if (!client->isValid())
        {
            std::cout << "client need to be authenticated" << std::endl;
            if (auth(*client, *message) == false)
            {
                ret = false;
                deleteClient(client->getId());
                break;
            }
        }
        else
            exec(*client, *message);
    }

        
    return ret;
}

void Server::connectionLoop()
{
    fd_set ready_connections_read, ready_connections_write;
    timeval time;
    time.tv_sec = 0;
    time.tv_usec = 500;
    // initialize the fd_set
    FD_ZERO(&_connections_set_read);
    FD_ZERO(&_connections_set_write);
    FD_SET(_server_socket, &_connections_set_read);
    FD_SET(_server_socket, &_connections_set_write);

    

    while (!_interrupt)
    {
        // the fd_set is always destroyed by the select() method
        ready_connections_read = _connections_set_read;
        ready_connections_write = _connections_set_write;
        int ret = select(FD_SETSIZE, &ready_connections_read, &ready_connections_write, NULL, &time);
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
            if (!FD_ISSET(fd, &ready_connections_read))
                continue ;
            inspectEvent(fd); // Note: inspectEvent() validates clientFd
            ready_connections_read = _connections_set_read;
            ready_connections_write = _connections_set_write;
        }

        for (int fd = 3; fd < FD_SETSIZE; fd++)
        {
            // if fd is not ready for reading, go to next one
            if (!FD_ISSET(fd, &ready_connections_write))
                continue ;
            
            ConnectionsList::iterator client = getClient(fd);
            if (client != _connections.end())
            {
                client->replyMessage();
            }
        }
    }
}

std::string    Server::readMessage(int fd) const
{
    static char buffer[BUFFER_SIZE];
    bzero(buffer, BUFFER_SIZE);

    int bytesReceived = recv(fd, buffer, BUFFER_SIZE, 0);

    // Log
    if (bytesReceived < 0 && errno != EBADF)
    {
        std::cerr << strerror(errno) << std::endl;
        std::cerr << "Failed to read message from client [fd " << fd << "]" << std::endl;
    }

    return buffer;
}
