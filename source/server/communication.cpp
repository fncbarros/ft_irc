/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   communication.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshintak <bshintak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 14:47:11 by falmeida          #+#    #+#             */
/*   Updated: 2023/09/24 19:27:11 by bshintak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

void    Server::channelPrivateMessage(const Client& client, const std::string& channelname, const std::string& message)
{
    ChannelsList::const_iterator channel = getChannel(channelname);

    if (channel == _channels.end())
    {
        replyChannelNotFound(client, channelname);
    }
    else
    {
        const ClientMap& clientMap = channel->getClients();
        for(ClientMap::const_iterator itClient = clientMap.begin(); itClient != clientMap.end(); itClient++)
        {
            if (itClient->second != &client)
            {
                replyChannelMessage(*itClient->second, client, channel->getName(), message);
            }
        }
    }
}

void    Server::clientPrivateMessage(const Client& client, const std::string& nickname, const std::string& message)
{
    ConnectionsList::const_iterator newClient = getClient(nickname);

    if (newClient == _connections.end())
    {
        replyPrivMessageNickNotFound(client, nickname);
    }
    else
    {
        replyPrivateMessage(client, *newClient, message);
    }
}

void    Server::channelNotice(const Client& client, const std::string& channelname, const std::string& message)
{
    ChannelsList::const_iterator channel = getChannel(channelname);

    if (channel == _channels.end())
    {
        replyNoSuchNick(client, channelname);
    }
    else
    {
        const ClientMap& clientMap = channel->getClients();
        for(ClientMap::const_iterator itClient = clientMap.begin(); itClient != clientMap.end(); itClient++)
        {
            if (itClient->second != &client)
            {
                replyChannelMessageNotice(*itClient->second, client, channel->getName(), message);
            }
        }
    }
}