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
    {
        checkPassword(client, processedMsg);
    }
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

void    Server::checkPassword(Client& client, const tokenPair &processedMsg)
{
    if (processedMsg.first.compare("PASS") || processedMsg.second.empty())
    {
        replyPassNeedMorParams(client);
    }
    else if (processedMsg.second != _password)
    {
        replyPassMissMatch(client);
    }
    else
    {
        client.setPassActive();
    }
}

void    Server::checkUser(Client& client, tokenPair processedMsg)
{
    if (!processedMsg.first.compare("NICK"))
    {
        execNICK(client, processedMsg.second);
    }
    else if (!processedMsg.first.compare("USER"))
    {
        execUSER(client, processedMsg.second);
    }
    else
    {
        std::cout << "Authentication needed\n" << std::endl;
        replyNotRegistered(client);
    }
}


void    Server::activateClient(Client& client)
{
    replyWelcome(client);
    replyYourHost(client);
    replyCreated(client);
    replyMyInfo(client);
}
