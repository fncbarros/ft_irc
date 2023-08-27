#include "../includes/Server.hpp"

bool Server::auth(Client& client, tokenList processedMsg)
{
    if (!client.isPassActive())
        return checkPassword(client, processedMsg);
  
    std::cout << "connection accepted" << std::endl;
    return true;
}

int    Server::checkPassword(Client& client, tokenList processedMsg)
{
    std::string password = getToken("PASS", processedMsg);
    std::cout << "["<< password << "] [" << _password << "]" << std::endl;
    if (password != _password)
        return replyPassMissMatch(client);
    
    std::cout << "pass correct" << std::endl;
    client.setPassActive();
    return true;
}