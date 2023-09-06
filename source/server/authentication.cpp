/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   authentication.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falmeida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 14:47:11 by falmeida          #+#    #+#             */
/*   Updated: 2023/08/12 14:47:13 by falmeida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

bool Server::auth(Client& client, const tokenPair &processedMsg)
{
    std::cout << "isuseractive " <<  client.isUserActive() << " isnickactive " << client.isNickActive() << std::endl;
    if (!client.isPassActive())
        return checkPassword(client, processedMsg);
    else if (!client.isUserActive() || !client.isNickActive())
    {
        checkUser(client, processedMsg);
    }    
    if (client.isValid())
    {
        activateClient(client);
        std::cout << "connection accepted" << std::endl;
    }    
    return true;
}

bool    Server::checkPassword(Client& client, const tokenPair &processedMsg)
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
    client.setPassActive();
    return true;
}

void    Server::checkUser(Client& client, tokenPair processedMsg)
{
    if (!processedMsg.first.compare("NICK"))
    {
        std::cout << "executing nick" << std::endl;
        execNICK(client, processedMsg.second);
        
    }
    else if (!processedMsg.first.compare("USER"))
    {
        std::cout << "executing user" << std::endl;
        execUSER(client, processedMsg.second);
    }
    else
    {
        std::cout << "Invalid Command" << std::endl;
    }
}


void    Server::activateClient(Client& client)
{
    replyYourHost(client);
    replyCreated(client);
    replyMyInfo(client);
}
