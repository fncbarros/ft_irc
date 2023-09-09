#include <Server.hpp>

void    Server::channelPrivateMessage(const Client& client, const std::string& nickname, const std::string& message)
{
    
    std::cout << "Private Message " << client.getNickname() << " user: [" << nickname << "] message: [" << message << "]" << std::endl;
}

void    Server::clientPrivateMessage(const Client& client, const std::string& nickname, const std::string& message)
{
    ConnectionsList::iterator newClient = getClient(nickname);

    if (newClient == _connections.end())
    {
        replyPrivMessageNickNotFound(client, nickname);
    }
    else
    {
        replyPrivateMessage(client, *newClient, message);
    }
    std::cout << "Private Message " << client.getNickname() << " user: [" << nickname << "] message: [" << message << "]" << std::endl;
}
