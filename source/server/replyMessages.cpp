/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replyMessages.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falmeida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 14:47:11 by falmeida          #+#    #+#             */
/*   Updated: 2023/08/12 14:47:13 by falmeida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

void    Server::replyWelcome(Client& client) const
{
    Utils::writeTo(":IRC42 NOTICE " + client.getNickname() + " :Welcome to the Internet Relay Network " + client.getNickname() + "!"  + client.getUsername() + "@IRC42\r\n", client.getId());
}

void Server::replyPassMissMatch(Client& client) const
{
    std::cout << "Incorrect pass" << std::endl;
    Utils::writeTo(":IRC42 " + PASSMISMATCH + " * :Password incorrect\r\n", client.getId());
}

void    Server::replyYourHost(Client& client) const
{
    Utils::writeTo(":IRC42 " + YOURHOST + " falmeida :Your host is IRC42, running version 1.0\r\n", client.getId());
}

void    Server::replyCreated(Client& client) const
{
    Utils::writeTo(":IRC42 " + CREATED + " falmeida :This server was created " + _server_date_created + "\r\n", client.getId());
}

void    Server::replyMyInfo(Client& client) const
{
    Utils::writeTo(":IRC42 " + MYINFO + " falmeida IRC42 1.0 0=o\r\n", client.getId());
}
