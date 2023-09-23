/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auxiliary.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbarros <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/4  14:47:11 by fbarros           #+#    #+#             */
/*   Updated: 2023/09/4  14:47:13 by fbarros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

ChannelsList::iterator Server::getChannel(const std::string& name)
{
    ChannelsList::iterator it =_channels.begin();
    for ( ; it != _channels.end(); it++)
    {
        if (it->getName() == name)
            break ;
    }
    return it;
}

ChannelsList::const_iterator Server::getChannel(const std::string& name) const
{
    ChannelsList::const_iterator it =_channels.begin();
    for ( ; it != _channels.end(); it++)
    {
        if (it->getName() == name)
            break ;
    }
    return it;
}

// For JOIN only
const std::string Server::returnChannelName(const std::string& line)
{
    size_t pos(line.find('#'));
    if (pos == std::string::npos)
    {
        std::cerr << "Error: join: could not find '#' token." << std::endl; // TODO: reply error
        return (BADJOIN + line.substr(0u, line.find(' ')));
    }
    return (line.substr(1));
}

bool    Server::channelExists(const std::string& name) const
{
    const ChannelsList::const_iterator it(getChannel(name));
    return (it !=_channels.end());
}

void    Server::broadcast(const std::string& msg, const std::string& channelName, const int exclude)
{
    ChannelsList::const_iterator chanIt = getChannel(channelName);
    if (chanIt == _channels.end())
        return ;

    const ClientMap& map(chanIt->getClients());
    for (ClientMap::const_iterator it = map.begin(); it != map.end(); it++)
    {
        const Client *client(it->second);
        if (client == NULL)
            break ;
        if (client->getId() != exclude)
        {
            addMessage(msg, client->getId());
        }
    }
}

void    Server::broadcast(const std::string& msg, const Channel& channel, const int exclude)
{
    const ClientMap& map(channel.getClients());
    for (ClientMap::const_iterator it = map.begin(); it != map.end(); it++)
    {
        const Client *client(it->second);
        if (client == NULL)
            break ;
        if (client->getId() != exclude)
        {
            addMessage(msg, client->getId());
        }
    }
}
