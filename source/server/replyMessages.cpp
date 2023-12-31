/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replyMessages.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshintak <bshintak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 14:47:11 by falmeida          #+#    #+#             */
/*   Updated: 2023/09/24 20:52:03 by bshintak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

void    Server::addMessage(const std::string& message, int fd)
{
    ConnectionsList::iterator client = getClient(fd);

    if (client != _connections.end())
    {
        client->setMessage(message);
    }
}

void    Server::replyWelcome(const Client& client)
{
    addMessage(":IRC42 " + WELCOME + " " + client.getNickname() + " :Welcome to the Internet Relay Network " + client.toString() + EOL, client.getId());
}

void Server::replyPassMissMatch(const Client& client)
{
    std::cout << "Incorrect pass" << std::endl;
    addMessage(":IRC42 " + PASSMISMATCH + " * :Password incorrect\r\n", client.getId());
}

void Server::replyPassNeedMorParams(const Client& client)
{
    addMessage(":IRC42 " + PASSNEEDPARAMS + " * :Not enough parameters\r\n", client.getId());
}

void    Server::replyYourHost(const Client& client)
{
    addMessage(":IRC42 " + YOURHOST + " " + client.getNickname() + " :Your host is IRC42, running version 1.0\r\n", client.getId());
}

void    Server::replyCreated(const Client& client)
{
    addMessage(":IRC42 " + CREATED + " " + client.getNickname() + " :This server was created " + _server_date_created + EOL, client.getId());
}

void    Server::replyMyInfo(const Client& client)
{
    addMessage(":IRC42 " + MYINFO + " " + client.getNickname() + " IRC42 1.0\r\n", client.getId());
}

void    Server::replyNickCollision(const Client& client)
{
    addMessage(":IRC42 " + NICKCOLLISION + " *" + client.getNickname() + " :Nickname is already in use.\r\n", client.getId());
}

void    Server::replyPrivMessageNickNotFound(const Client& client, const std::string& targetNickName)
{
    addMessage(":IRC42 " + NICKNOTFOUND + " " + client.getNickname() + " " +  targetNickName + " :No such nickname\r\n", client.getId());
}

void    Server::replyPrivateMessage(const Client& client, const Client& targetClient, const std::string& message)
{
    addMessage(":" + client.toString() + " PRIVMSG " + targetClient.getNickname() + " " + message + EOL, targetClient.getId());
}

void    Server::replyChannelNotFound(const Client& client, const std::string &channelName)
{
    addMessage(":" + client.toString() + " " + NOSUCHCHANNEL + " " + client.getNickname() + " #" + channelName + " :No such channel\r\n", client.getId());
}

void    Server::replyChannelMessage(const Client& client,  const Client& clientSender, const std::string& channelName, const std::string& message)
{
    addMessage(":" + clientSender.toString() + " PRIVMSG #" + channelName + " " + message + EOL, client.getId());
}

void    Server::replyNoExternalChannelMessage(const Client& client, const std::string& channelName)
{
    addMessage(":IRC42 " + CANNOTSENDTOCHAN + " " + client.getNickname() + " #" + channelName + " :No external channel messages " + "(#" + channelName + ")" + "\r\n", client.getId());
}

void    Server::replyJoin(const int id, const Client& client, const Channel& channel)
{
    addMessage(":" + client.toString() + " JOIN #" + channel.getName() + " * :" + client.getNickname() + EOL,  id);
}

void    Server::replyName(const Client& client, const Channel& channel)
{
    // ( '=' / '*' / '@' ) <channel> ' ' : [ '@' / '+' ] <nick> *( ' ' [ '@' / '+' ] <nick> )

    // first half
    addMessage(":" + HOST + " " + NAMREPLY + " " + client.getNickname() + " = #" + channel.getName() + " :@", client.getId());

    // list users
    const ClientMap map = channel.getClients();
    for (ClientMap::const_iterator it = map.begin(); it != map.end(); it++)
    {
        addMessage(it->second->toString() + " ", client.getId());
    }
    addMessage(EOL, client.getId());
  
}

void    Server::replyEndOfNames(const Client& client, const Channel& channel)
{
    addMessage(":" + HOST + " " + ENDOFNAMES + " " + client.getNickname() + " " +  channel.getName() + " " +  ":End of /NAMES list.\r\n", client.getId());
}

void    Server::replyChannelMode(const Client& client, const Channel& channel)
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
    if (channel.limit())
        modes += 'l';
    addMessage(":" + HOST + " " + CHANNELMODEIS + " " + client.getNickname() + " " +  channel.getName() + modes + EOL, client.getId());
}

void    Server::replyCreationTime(const Client& client, const Channel& channel)
{
    addMessage(":" + HOST + " " + CREATIONTIME + " " + client.getNickname() + " " +  channel.getName() + " " + Utils::timeToStr() + EOL, client.getId());
}

void    Server::replyBadJoin(const Client& client, const std::string& channel)
{
    addMessage(":" + HOST + " " + BADJOIN + " " + client.getNickname() + " " + channel + " :Cannot join channel: Channel name must start with a hash mark (#)\r\n", client.getId());
}

void    Server::replyNoSuchChannel(const Client& client)
{
    const int id(client.getId());
    const std::string nick(client.getNickname());
    addMessage(":" + HOST + " " + NOSUCHCHANNEL + " " + nick + " " + HOST + " :No such channel\r\n", id);
}

void    Server::replyNotInChannel(const Client& client, const std::string& userNick, const std::string& channelName)
{
    const int id(client.getId());
    const std::string nick(client.getNickname());
    addMessage(":" + HOST + " " + USERNOTINCHANNEL + " " + nick + " " + userNick + " #" + channelName + " :They aren't on that channel\r\n", id);
}

void    Server::replyNoPriviledges(const Client& client, const std::string& channelName)
{
    const int id(client.getId());
    const std::string nick(client.getNickname());
    addMessage(":" + HOST + " " + CHANOPRIVSNEEDED + " " + nick + " #" + channelName + " :You're not channel operator\r\n", id);
}

void    Server::replyKick(const Client& client, const Client& kicker, const Channel& channel, const std::string& userNick, const std::string& reason)
{
    const int id(client.getId());
    const std::string nick(kicker.getNickname());
    const std::string channelName(channel.getName());
    addMessage(":" + kicker.toString() + " " + "KICK" + " #" + channelName + " " + userNick + " :" + reason + EOL, id);
}

void    Server::replyBroadcastKick(const int id, const std::string& kickerNick, const std::string& userNick, const std::string& channelName, const std::string& reason)
{
    addMessage(kickerNick + " has kicked " + userNick + " from #" + channelName + " :" + reason + EOL, id);
}

void    Server::replyNoSuchNick(const Client& client, const std::string& nickTarget)
{
    addMessage(":" + HOST + " " + NICKNOTFOUND + " " + client.getNickname() + " " + nickTarget + " :No such nick/channel\r\n", client.getId());
}

void    Server::replyNotOnChannelError(const Client& client, const std::string& channelName)
{
    addMessage(":" + HOST + " " + CLIENTNOTONCHANNEL + " " + client.getNickname() + " #" + channelName + " :You're not on that channel\r\n", client.getId());
}

void    Server::replyClientTargetOnChannel(const Client& client, const std::string& nickTarget, const std::string& channelName)
{
    addMessage(":" + HOST + " " + CLIENTONCHANNEL + " " + client.getNickname() + " " + nickTarget + " #" + channelName + " :is already on channel\r\n", client.getId());
}

void    Server::replyInviting(const Client& client, const std::string& nickTarget, const std::string& channelName)
{
    addMessage(":" + HOST + " " + INVITING + " " + client.getNickname() + " " + nickTarget + " :#" + channelName + EOL, client.getId());
}

void    Server::replyInvitingReceived(const Client& client, const Client& clientTarget, const std::string& channelName)
{
    addMessage(":" + client.toString() + " INVITE " + clientTarget.getNickname() + " :#" + channelName + EOL, clientTarget.getId());
}

void    Server::replyNotOnChannel(const Client& client, const std::string& channelName)
{
    const int id(client.getId());
    const std::string nick(client.getNickname());
    addMessage(client.toString() + " " + NOTONCHANNEL + " " + nick + " " + "#" + channelName + " :You're not on that channel\r\n", id);
}

void    Server::replyPart(const Client& client, const std::string& channelName)
{
    const int id(client.getId());
    addMessage(":" + client.toString() + " PART #" + channelName + EOL, id);
}

void    Server::replyYouWereKicked(const int id, const std::string& channelName, const std::string& kickerNick, const std::string& reason)
{
        addMessage("You were kicked from #" + channelName + " by " + kickerNick + " (" + reason + ")\r\n", id);
}

void    Server::replyPartUsage(const int id)
{
    addMessage("Usage: PART [<channel>] [<reason>], leaves the channel, by default the current one", id);
}

void    Server::replyNoSuchChannelSimple(const int id, const std::string& channelName)
{
    addMessage(channelName + " :No such channel\r\n", id);
}

void    Server::replyYouLeftChannel(const int id, const std::string& channelName, const std::string& reason)
{
    addMessage("You have left channel #<" + channelName + "> (" + reason + ")\r\n", id);
}

void    Server::replyBroadcastUserLeft(const int id, const Client& client, const std::string& reason)
{
    addMessage(client.getNickname() + " (" + client.getUsername() + "@" + HOST + ") has left (" + reason + ")\r\n", id);
}

void    Server::replyTopic(const Client& client, const Channel& channelTarget)
{
    broadcast(":" + client.toString() + " TOPIC #" + channelTarget.getName() + " :" + channelTarget.getTopic() + "\r\n", channelTarget);
}

void    Server::replyNoTopic(const Client& client, const Channel& channelTarget, const std::string nickTopic)
{
    const int id(client.getId());
    // addMessage(":" + client.toString() + " " + TOPIC + " " + client.getNickname() + " #" + channelTarget.getName() + " :" + channelTarget.getTopic() + "\r\n", id);
    addMessage(":" + client.toString() + " " + TOPICWHOTIME + " " + client.getNickname() + " #" + channelTarget.getName() + " " + nickTopic + "\r\n", id);
}

void    Server::replyNoTopicSet(const Client& client, const Channel& channelTarget)
{
    const int id(client.getId());
    addMessage(":" + client.toString() + " " + NOTOPIC + " " + client.getNickname() + " #" + channelTarget.getName() + " :No topic is set." + "\r\n", id);
}

void   Server::replyChanopNeeded(const Client& client, const std::string& channel, const std::string& msg)
{
    const std::string firstHalf(":" + HOST + " " + CHANOPRIVSNEEDED + " " + client.getNickname() + " #" + channel + " :");
    addMessage(firstHalf + msg + EOL, client.getId());
}

void Server::replyMissingParam(const Client& client, const std::string& channel, const std::string& param)
{
    const std::string REPLYCODE("696");
    addMessage(":" + HOST + " " + REPLYCODE + " " + client.getNickname() + " #" + channel + " " + param + " * :You must specify a parameter for the op mode. Syntax: <nick>.\r\n", client.getId());
}

void Server::replyChannelModeIs(const Client& client, const Channel& channel)
{
	const int id(client.getId());
	const int limit(channel.limit());
    std::string limitNum = (limit > 0) ? (" :" + Utils::numToStr(limit)) : "";
    std::string modes;

    if (limitNum.empty())
        modes = ":";
    modes += channel.returnModes();
	addMessage(":" + HOST + " " + CHANNELMODEIS + " " + client.getNickname() + " #" + channel.getName() +  " :" + channel.returnModes() + limitNum + EOL, id);
}

void    Server::replyInviteOnly(const Client& client, const std::string& channel)
{
    addMessage(":" + HOST + " " + INVITEONLYCHAN + " " + client.getNickname() + " #" + channel + " :Cannot join channel (+i)\r\n", client.getId());
}

void    Server::replyNoChannelJoined(const Client& client)
{
    addMessage(":" + HOST + " " + NOTONCHANNEL + " " + client.getNickname() + " :No channel joined. Try /join #<channel>\r\n", client.getId());
}

void    Server::replyModeMissingParams(const int id)
{
    addMessage("MODE :<target> [[(+|-)]<modes> [<mode-parameters>]]\r\n", id);
}

void Server::replyModeUnknown(const Client& client, const std::string& param)
{
    const int id(client.getId());
    const std::string nick(client.getNickname());
    addMessage(":" + HOST + " " + UNKNOWNMODE + " " + nick + " " + param + " " + " :is an unknown mode character\r\n" ,id);
}

void Server::replyMode(const Client& client, const std::string& channel, const std::string& param1, const std::string& param2)
{
    const std::string nick(client.getNickname());

    // "Official" reply
    if (param1 == "+l")
        broadcast(":" + client.toString() + " MODE #" + channel + " " + param1 + " :" + param2 + EOL, channel);
    else
        broadcast(":" + client.toString() + " MODE #" + channel + " :" + param1 + EOL, channel);
}

void    Server::replyNoticePriv(const Client& client, const std::string& message, const std::string& channel, const Client& newClient)
{
    addMessage(":" + client.toString() + " NOTICE " + channel + " " + message + "\r\n", newClient.getId());
}

void    Server::replyChannelMessageNotice(const Client& client,  const Client& clientSender, const std::string& channelName, const std::string& message)
{
    addMessage(":" + clientSender.toString() + " NOTICE #" + channelName + " " + message + "\r\n", client.getId());
}
void Server::replyChannelIsFull(const Client& client, const std::string& channel)
{
    addMessage(":" + HOST + " " + CHANNELISFULL + " " + client.getNickname() + " #" + channel + " :Cannot join channel (+l)\r\n", client.getId());
}

void Server::replyNotRegistered(const Client& client)
{
    addMessage(":"+ HOST + " " + NOTREGISTERED + " " +  client.getNickname() + " :You have not registered.\r\n", client.getId());
}

void Server::replyAway(const std::string& nick, const std::string& message)
{
    broadcast(":" + HOST + " " + AWAY + " " + nick + " " + message);
}
