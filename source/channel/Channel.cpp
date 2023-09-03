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

#include <Channel.hpp>

Channel::Channel(const std::string name, const Client& client)
: _name(name)
{
    _users.push_back(std::make_pair(&client, true));
}

Channel::~Channel()
{
    // broadcast to all users
    for (UserList::iterator it = _users.begin(); it != _users.end(); it++)
    {
        Utils::writeTo("Channel " + _name + " has been removed.\n", it->first->getId());
    }
}

std::string Channel::getName() const
{
    return _name;
}

UserList Channel::getList() const
{
    return _users;
}

void Channel::addClient(const Client& client)
{
    _users.push_back(std::make_pair(&client, false));
}

void Channel::printList(int fd) const
{
    std::string nameList;
    for (UserList::const_iterator it = _users.begin(); it != _users.end(); it++)
    {
        nameList += it->first->getNickname() + "\n";
    }
    Utils::writeTo(nameList, fd);
}
