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
    std::cout << "Channel " << name << " created." << std::endl;
}

Channel::~Channel()
{
    // broadcast to all users
    for (UserList::iterator it = _users.begin(); it != _users.end(); it++)
    {
//        Utils::writeTo("Channel " + _name + " has been removed.\n", it->first->getId());
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

bool Channel::addClient(const Client& client)
{
    if (_users.size() < _modes.limit || !limit())
    {
        _users.push_back(std::make_pair(&client, false));
        return true;
    }
    else
    {
        return false;
    }
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

Channel::modes::modes()
: invite_only(false)
, topic_restricted(false)
, key(std::make_pair(false, ""))
, operator_privs(false)
, limit(0)
{
}

Channel::modes::modes(const modes &other)
{
    invite_only = other.invite_only;
    topic_restricted = other.topic_restricted;
    key = other.key;
    operator_privs = other.operator_privs;
    limit = other.limit;
}

Channel::modes& Channel::modes::operator=(const modes &other)
{
    if (this != &other)
    {
        *this = other;
    }
    return *this;
}

bool Channel::isInviteOnly(void) const
{
    return _modes.invite_only;
}

bool Channel::isTopicRetricted(void) const
{
    return _modes.topic_restricted;
}

bool Channel::hasKey(void) const
{
    return _modes.key.first;
}

bool Channel::hasOperatorPriviledges(void) const
{
    return _modes.operator_privs;
}

size_t Channel::limit(void) const
{
    return _modes.limit;
}

bool Channel::hasModes(void) const
{
    return (isInviteOnly() || isTopicRetricted() || hasKey() || hasOperatorPriviledges() || limit());
}

void Channel::setInviteOnly(const bool set)
{
    _modes.invite_only = set;
}

void Channel::setTopicRestriction(const bool set)
{
    _modes.topic_restricted = set;
}

void Channel::setKey(const std::string& key)
{
    _modes.key.first = true;
    _modes.key.second = key;
}

void Channel::setNoKey(void)
{
    _modes.key.first = false;
    _modes.key.second.clear();
}

void Channel::setPriviledges(const bool set)
{
    _modes.operator_privs = set;
}

void Channel::setLimit(const size_t limit)
{
    _modes.limit = limit;
}
