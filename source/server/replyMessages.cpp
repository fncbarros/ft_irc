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
    Utils::writeTo(":IRC42 " + YOURHOST + " " + client.getNickname() + " :Your host is IRC42, running version 1.0\r\n", client.getId());
}

void    Server::replyCreated(Client& client) const
{
    Utils::writeTo(":IRC42 " + CREATED + " " + client.getNickname() + " :This server was created " + _server_date_created + "\r\n", client.getId());
}

void    Server::replyMyInfo(Client& client) const
{
    Utils::writeTo(":IRC42 " + MYINFO + " " + client.getNickname() + " IRC42 1.0 0=o\r\n", client.getId());
}

void    Server::replyNickCollision(Client& client) const
{
    Utils::writeTo(":IRC42 " + NICKCOLLISION + " *" + client.getNickname() + " :Nickname is already in use.\r\n", client.getId());
}

void    Server::replyPrivMessageNickNotFound(Client& client, const std::string &targetNickName) const
{
    Utils::writeTo(":IRC42 " + NICKNOTFOUND + " " + client.getNickname() + " " +  targetNickName + " :No such nickname\r\n", client.getId());
}

void    Server::replyPrivateMessage(Client& client, Client& targetCLient,const std::string message)
{
    Utils::writeTo(":IRC42 PRIVMSG" + client.getNickname() + " :" + message + "\r\n", targetCLient.getId()); 
}