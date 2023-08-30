#include "../includes/Server.hpp"
#include "../includes/common.hpp"

void    Server::replyPassMissMatch(Client& client)
{
    std::cout << "Incorrect pass" << std::endl;
    Utils::writeTo(":IRC42 " + PASSMISMATCH + " * :Password incorrect\r\n", client.getId());
}

void    Server::replyWellcome(Client& client)
{
    Utils::writeTo(":IRC42 NOTICE falmeida :Welcome to the Internet Relay Network falmeida!fbarros@IRC42\r\n", client.getId());
}

void    Server::replyYourHost(Client& client)
{
    Utils::writeTo(":IRC42 " + YOURHOST + " falmeida :Your host is IRC42, running version 1.0\r\n", client.getId());
}

void    Server::replyCreated(Client& client)
{
    Utils::writeTo(":IRC42 " + CREATED + " falmeida :This server was created " + _server_date_created + "\r\n", client.getId());
}

void    Server::replyMyInfo(Client& client)
{
    Utils::writeTo(":IRC42 " + MYINFO + " falmeida IRC42 1.0 0=o\r\n", client.getId());
}

std::string    Server::readMessage(int fd) const
{
    char buffer[BUFFER_SIZE] = {0};
    bzero(buffer, BUFFER_SIZE);

    int bytesReceived = recv(fd, buffer, BUFFER_SIZE, 0);

    if (bytesReceived < 0)
        std::cout << "Failed to read Client Socket" << std::endl;

    std::cout << buffer << std::endl;
    std::cout << "Client message received" << std::endl;
    return buffer;
}

void    Server::sendMessage(int fd, const std::string message)
{
    send(fd, message.c_str(), message.size(), 0);
}