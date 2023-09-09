/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replyMessages.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshintak <bshintak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 14:47:11 by falmeida          #+#    #+#             */
/*   Updated: 2023/09/09 15:15:48 by bshintak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

void    Server::replyWelcome(const Client& client) const
{
    Utils::writeTo(":IRC42 " + WELCOME + " " + client.getNickname() + " :Welcome to the Internet Relay Network " + client.toString() + "\r\n", client.getId());
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

void    Server::replyPrivMessageNickNotFound(const Client& client, const std::string &targetNickName) const
{
    Utils::writeTo(":IRC42 " + NICKNOTFOUND + " " + client.getNickname() + " " +  targetNickName + " :No such nickname\r\n", client.getId());
}

void    Server::replyPrivateMessage(const Client& client, const Client& targetClient,const std::string message) const
{
    Utils::writeTo(":" + client.toString() + " PRIVMSG " + targetClient.getNickname() + " " + message + "\r\n", targetClient.getId());
}

void    Server::replyCAPLS(Client& client, std::string capabilities) const
{
    Utils::writeTo(":IRC42 " + capabilities + "\r\n", client.getId());
}

void    Server::replyJoin(const Client& client, const Channel& channel) const
{
    Utils::writeTo(":" + client.toString() + " JOIN #" + channel.getName() + " * \r\n", client.getId());
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

void    Server::replyEndOfNames(const Client& client, const Channel& channel) const
{
    Utils::writeTo(":" + HOST + " " + ENDOFNAMES + " " + client.getNickname() + " " +  channel.getName() + " " +  ":End of /NAMES list.\r\n", client.getId());
}

void    Server::replyChannelMode(const Client& client, const Channel& channel) const
{
    std::string modes(" ");
    if (channel.hasModes())
        modes += '+';
    if (channel.isInviteOnly())
        modes += 'i';
    if (channel.isTopicRetricted())
        modes += 't';
    if (channel.hasKey())
        modes += 'k';
    if (channel.hasOperatorPriviledges())
        modes += 'o';
    if (channel.limit())
        modes += 'l';
    Utils::writeTo(":" + HOST + " " + CHANNELMODEIS + " " + client.getNickname() + " " +  channel.getName() + modes + "\r\n", client.getId());
}

void    Server::replyCreationTime(const Client& client, const Channel& channel) const
{
    Utils::writeTo(":" + HOST + " " + CREATIONTIME + " " + client.getNickname() + " " +  channel.getName() + " " + Utils::timeToStr() + "\r\n", client.getId());
}

void    Server::replyWho(const Client& client, const Channel& channel) const
{
    // :<server_name> 354 <nick> <channel> <username> <host> <server> <nick> <flags> :<hopcount> <realname>
    // :irc.example.com 354 MyNickname #example User1 host.server.com OtherUser +i :1 John Doe
    // TODO: figure out second half of reply
    Utils::writeTo( ":" + HOST + " " + WHOSPCRPL + " " + client.getNickname() + " " + channel.getName() + " "
            +  client.getUsername() + " " +  HOST + " " + /* <server>  <nick> <flags> :<hopcount> <realname> */
            + "\r\n", client.getId());
}

void    Server::replyEndOfWho(const Client& client, const Channel& channel) const
{
    Utils::writeTo(":" + HOST + " " + ENDOFWHO + " " + client.getNickname() + " " + channel.getName() + " " +
                   ":End of /WHO list\r\n", client.getId());
}

void    Server::replyBadJoin(const Client& client, const std::string& line) const
{
    // :irc4.acc.umu.se 448 teeee . :Cannot join channel: Channel name must start with a hash mark (#)
    Utils::writeTo(":" + HOST + " " + BADJOIN + " " + client.getNickname() + " " + line + " :Cannot join channel: Channel name must start with a hash mark (#)\r\n", client.getId());
}
