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
        return false;
    std::cout << "["<< processedMsg.second << "] [" << _password << "]" << std::endl;
    if (processedMsg.second != _password)
        return replyPassMissMatch(client);
    
    std::cout << "pass correct" << std::endl;
    client.setPassActive();
    return true;
}