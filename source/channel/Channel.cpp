/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbarros <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 19:12:25 by fbarros           #+#    #+#             */
/*   Updated: 2023/08/09 19:13:22 by fbarros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"

Channel::Channel(const std::string name, const Client& client)
: _name(name)
{
    _clientList.push_back(&client);
}

Channel::~Channel ()
{
    std::cout << "Channel " << _name << " has been removed.\n";
    // broadcast to all users
}

std::string Channel::getName() const
{
    return _name;
}

// TODO: return iterator to 1st element instead maybe
// TODO: overload function to be more helpful (like returning list of Channels
//  param Client/id/nickname is part of)
ClientList  Channel::getList() const
{
    return _clientList;
}

void        Channel::addClient(const Client& client)
{
    _clientList.push_back(&client);
}
