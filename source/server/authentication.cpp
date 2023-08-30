#include "../includes/Server.hpp"

bool Server::auth(Client& client, tokenList processedMsg)
{
    if (!client.isPassActive())
        return checkPassword(client, processedMsg);
  
    std::cout << "connection accepted" << std::endl;
    return true;
}

bool    Server::checkPassword(Client& client, tokenList processedMsg)
{
    std::string password = getToken("PASS", processedMsg);
    std::cout << "["<< password << "] [" << _password << "]" << std::endl;
    if (password != _password)
    {
        replyPassMissMatch(client);
        return false;
    }
    
    //Active client need to be moved to auth after tokenkList refactor
    activateClient(client);
    return true;
}

void    Server::activateClient(Client& client)
{
    replyWellcome(client);
    replyYourHost(client);
    replyCreated(client);
    replyMyInfo(client);
    client.setPassActive();
}
