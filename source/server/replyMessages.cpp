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

void    Server::replyWelcome(const Client& client) const
{
    Utils::writeTo(":IRC42 NOTICE " + client.getNickname() + " :Welcome to the Internet Relay Network " + client.toString() + "\r\n", client.getId());
}

void Server::replyPassMissMatch(const Client& client) const
{
    std::cout << "Incorrect pass" << std::endl;
    Utils::writeTo(":IRC42 " + PASSMISMATCH + " * :Password incorrect\r\n", client.getId());
}

void    Server::replyYourHost(const Client& client) const
{
    Utils::writeTo(":IRC42 " + YOURHOST + " " + client.getNickname() + " :Your host is IRC42, running version 1.0\r\n", client.getId());
}

void    Server::replyCreated(const Client& client) const
{
    Utils::writeTo(":IRC42 " + CREATED + " " + client.getNickname() + " :This server was created " + _server_date_created + "\r\n", client.getId());
}

void    Server::replyMyInfo(const Client& client) const
{
    Utils::writeTo(":IRC42 " + MYINFO + " " + client.getNickname() + " IRC42 1.0 0=o\r\n", client.getId());
}

void    Server::replyNickCollision(const Client& client) const
{
    Utils::writeTo(":IRC42 " + NICKCOLLISION + " *" + client.getNickname() + " :Nickname is already in use.\r\n", client.getId());
}

void    Server::replyPrivMessageNickNotFound(Client& client, const std::string &targetNickName) const
{
    Utils::writeTo(":IRC42 " + NICKNOTFOUND + " " + client.getNickname() + " " +  targetNickName + " :No such nickname\r\n", client.getId());
}

void    Server::replyPrivateMessage(Client& client, Client& targetClient,const std::string message)
{
    Utils::writeTo(":" + client.getNickname() + "!" + client.getUsername() + "@0.0.0.0 PRIVMSG " + targetClient.getNickname() + " " + message + "\r\n", targetClient.getId()); 
}

void    Server::replyName(const Client& client, const Channel& channel) const
{
    //  :irc.eagle.y.se 353 T-bone = #new :T-bone!timey@180.226.108.93.rev.vodafone.pt barney!barney@180.226.108.93.rev.vodafone.pt @Nico2!fbarrosdff@180.226.108.93.rev.vodafone.pt
    // ( '=' / '*' / '@' ) <channel> ' ' : [ '@' / '+' ] <nick> *( ' ' [ '@' / '+' ] <nick> )

    // first half
    Utils::writeTo(":" + HOST + " " + NAMREPLY + " " + client.getNickname() + " = #" + channel.getName() + " :@", client.getId());


    // list users
    const UserList list = channel.getList();
    for (UserList::const_iterator it = list.begin(); it != list.end(); it++)
        Utils::writeTo(it->first->toString() + " ", client.getId());
    Utils::writeTo("\r\n", client.getId());

}

//:T-bone!timey@180.226.108.93.rev.vodafone.pt JOIN #new * :realname

// Raw:
// :Nico2!fbarrosdff@180.226.108.93.rev.vodafone.pt MODE #new +o barney
//  :Nico2!fbarrosdff@180.226.108.93.rev.vodafone.pt MODE #new +o barney
// in chat:
//Nico2 gives channel operator status to barney

