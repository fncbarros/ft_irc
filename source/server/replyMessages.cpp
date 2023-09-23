/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replyMessages.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshintak <bshintak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 14:47:11 by falmeida          #+#    #+#             */
/*   Updated: 2023/09/23 16:46:35 by bshintak         ###   ########.fr       */
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
    addMessage(":IRC42 " + WELCOME + " " + client.getNickname() + " :Welcome to the Internet Relay Network " + client.toString() + "\r\n", client.getId());
}

void Server::replyPassMissMatch(const Client& client)
{
    std::cout << "Incorrect pass" << std::endl;
    addMessage(":IRC42 " + PASSMISMATCH + " * :Password incorrect\r\n", client.getId());
}

void    Server::replyYourHost(const Client& client)
{
    addMessage(":IRC42 " + YOURHOST + " " + client.getNickname() + " :Your host is IRC42, running version 1.0\r\n", client.getId());
}

void    Server::replyCreated(const Client& client)
{
    addMessage(":IRC42 " + CREATED + " " + client.getNickname() + " :This server was created " + _server_date_created + "\r\n", client.getId());
}

void    Server::replyMyInfo(const Client& client)
{
    addMessage(":IRC42 " + MYINFO + " " + client.getNickname() + " IRC42 1.0 0=o\r\n", client.getId());
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
    addMessage(":" + client.toString() + " PRIVMSG " + targetClient.getNickname() + " " + message + "\r\n", targetClient.getId());
}

void    Server::replyChannelNotFound(const Client& client, const std::string &channelName)
{
    addMessage(":" + client.toString() + " " + NOSUCHCHANNEL + " " + client.getNickname() + " #" + channelName + " :No such channel\r\n", client.getId());
}

void    Server::replyChannelMessage(const Client& client,  const Client& clientSender, const std::string& channelName, const std::string& message)
{
    addMessage(":" + clientSender.toString() + " PRIVMSG #" + channelName + " " + message + "\r\n", client.getId());
}

void    Server::replyCAPLS(Client& client, std::string capabilities)
{
    addMessage(":IRC42 " + capabilities + "\r\n", client.getId());
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
    addMessage("\r\n", client.getId());
  
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
    addMessage(":" + HOST + " " + CHANNELMODEIS + " " + client.getNickname() + " " +  channel.getName() + modes + "\r\n", client.getId());
}

void    Server::replyCreationTime(const Client& client, const Channel& channel)
{
    addMessage(":" + HOST + " " + CREATIONTIME + " " + client.getNickname() + " " +  channel.getName() + " " + Utils::timeToStr() + EOL, client.getId());
}

void    Server::replyWho(const Client& client, const Channel& channel)
{
    // :<server_name> 354 <nick> <channel> <username> <host> <server> <nick> <flags> :<hopcount> <realname>
    // TODO: figure out second half of reply
    addMessage( ":" + HOST + " " + WHOSPCRPL + " " + client.getNickname() + " " + channel.getName() + " "
            +  client.getUsername() + " " +  HOST + " " + /* <server>  <nick> <flags> :<hopcount> <realname> */
            + "\r\n", client.getId());
}

void    Server::replyEndOfWho(const Client& client, const Channel& channel)
{
    addMessage(":" + HOST + " " + ENDOFWHO + " " + client.getNickname() + " " + channel.getName() + " " +
                   ":End of /WHO list\r\n", client.getId());
}

void    Server::replyBadJoin(const Client& client, const std::string& line)
{
    addMessage(":" + HOST + " " + BADJOIN + " " + client.getNickname() + " " + line + " :Cannot join channel: Channel name must start with a hash mark (#)\r\n", client.getId());
}

void    Server::replyList(const Client& client)
{
    std::string numberOfChannels = Utils::numToStr(_channels.size());

    addMessage(':' + HOST + " " + LISTSTART + " " + client.getNickname() + " :Channel :Users Name\r\n" , client.getId());
    for (ChannelsList::const_iterator it = _channels.begin(); it != _channels.end(); it++)
    {
        std::string numberOfUsers = Utils::numToStr(it->getClients().size());

        addMessage(":" + HOST + " " + LIST + " " + client.getNickname() + " #" + it->getName() + " " + numberOfUsers + " :" + it->getTopic() + "\r\n", client.getId());
    }
    addMessage(":" + HOST + " " + LISTEND + " " + client.getNickname() + " :End of /LIST\r\n", client.getId());
}

void    Server::replyList(const Client& client, const Channel& channel)
{
    (void)channel;
    (void)client;
    // addMessage(':' + HOST + " " + LISTSTART + " " + client.getNickname() + " :Channel list - " + numberOfChannels + " channels\r\n" , client.getId());
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
    const std::string reply("#" + channelName + " :You're not channel operator\r\n");
    Utils::writeTo(reply, id);
    Utils::writeTo(":" + HOST + " " + CHANOPRIVSNEEDED + " " + nick + " " + reply, id);
}

void    Server::replyKick(const Client& client, const Client& kicker, const Channel& channel, const std::string& userNick, const std::string& reason)
{
    const int id(client.getId());
    const std::string nick(kicker.getNickname());
    const std::string channelName(channel.getName());
    Utils::writeTo(":" + kicker.toString() + " " + "KICK" + " #" + channelName + " " + userNick + " :" + reason + EOL, id);
}

void    Server::replyBroadcastKick(const int id, const std::string& kickerNick, const std::string& userNick, const std::string& channelName, const std::string& reason)
{
    Utils::writeTo(kickerNick + " has kicked " + userNick + " from #" + channelName + " :" + reason + EOL, id);
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
    addMessage(":" + HOST + " " + INVITING + " " + client.getNickname() + " " + nickTarget + " :#" + channelName + "\r\n", client.getId());
}

void    Server::replyInvitingReceived(const Client& client, const Client& clientTarget, const std::string& channelName)
{
    addMessage(":" + client.toString() + " INVITE " + clientTarget.getNickname() + " :#" + channelName + "\r\n", clientTarget.getId());
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
    addMessage(":" + client.toString() + " PART #" + channelName + "\r\n", id);
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
    const int id(client.getId());
    addMessage(":" + client.toString() + " TOPIC #" + channelTarget.getName() + " :" + channelTarget.getTopic() + "\r\n", id);
}

void    Server::replyNotChannelOperatorTopic(const Client& client, const std::string& channelTargetName)
{
    const int id(client.getId());
    addMessage(HOST + " " + CHANOPRIVSNEEDED + " " + client.getNickname() + " " + channelTargetName + " :You're not a channel operator", id);
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
    addMessage("#" + channel + " " + param + " * :You must specify a parameter for the op mode. Syntax: <nick>.\r\n", client.getId());
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
