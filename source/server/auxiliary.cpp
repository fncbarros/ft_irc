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

ChannelsList::const_iterator Server::getChannel(const std::string& name) const
{
    ChannelsList::const_iterator it;
    for (it = _channels.begin(); it != _channels.end(); it++)
    {
        if (it->getName() == name)
            break ;
    }
    return it;
}

const std::string Server::returnChannelName(const std::string& line)
{
    size_t pos(line.find('#'));
    if (pos == std::string::npos)
        std::cerr << "Error: join: could not find '#' token." << std::endl; // TODO: reply error
    return (line.substr(pos + 1));
}