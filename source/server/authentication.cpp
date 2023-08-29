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

bool Server::auth(Client& client, tokenList processedMsg)
{
    if (client.isPassActive())
    {
        return true;
    }
    else if (checkPassword(client, processedMsg))
    {
        std::cout << "connection accepted" << std::endl;
        return true;
    }
    else
    {
        return false;
    }
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
    replyWelcome(client);
    replyYourHost(client);
    replyCreated(client);
    replyMyInfo(client);
    client.setPassActive();
}


