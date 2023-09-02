#include "../includes/Server.hpp"

bool Server::auth(Client& client, tokenPair processedMsg)
{
    if (!client.isPassActive())
        return checkPassword(client, processedMsg);
  
    std::cout << "connection accepted" << std::endl;
    return true;
}

int    Server::checkPassword(Client& client, tokenPair processedMsg)
{
    if (processedMsg.first.compare("PASS"))
    {
        std::cout << "command received is not PASS" << std::endl;
        return false;
    }
    std::cout << "["<< processedMsg.second << "] [" << _password << "]" << std::endl;
    if (processedMsg.second != _password)
    {
        replyPassMissMatch(client);
        return false;
    }
    
    std::cout << "pass correct" << std::endl;
    activateClient(client);
    return true;
}

void    Server::activateClient(Client& client)
{
    replyWelcome(client);
    replyYourHost(client);
    replyCreated(client);
    replyMyInfo(client);
    client.setPassActive();
}