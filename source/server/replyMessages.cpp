#include "../includes/Server.hpp"
#include "../includes/common.hpp"

int Server::replyPassMissMatch(Client& client)
{
    std::cout << "Incorrect pass" << std::endl;
    Utils::writeTo(":IRC42 " + PASSMISMATCH + " * :Password incorrect\r\n", client.getId());
    return false;
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