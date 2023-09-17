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

Channel::Channel(const std::string name, const Client* client)
: _name(name)
{
    _clientsMap.insert(std::make_pair(client->getId(), client));
    std::cout << "Channel " << name << " created." << std::endl;
    addOperator(client->getId());
}

Channel::Channel(const Channel& other)
{
    *this = other;
}

Channel& Channel::operator=(const Channel& other)
{
    if (this != &other)
    {
        _name = other._name;
        _topic = other._topic;
        _modes = other._modes;
        for (ClientMap::const_iterator it = other._clientsMap.begin(); it != other._clientsMap.end(); it++)
        {
            _clientsMap[it->first] = it->second;
        }
        _operators = other._operators;
    }
    return *this;
}

Channel::~Channel()
{
    // TODO: broadcast to server?
}

std::string Channel::getName() const
{
    return _name;
}

const ClientMap& Channel::getClients() const
{
    return _clientsMap;
}

bool Channel::addClient(const Client& client)
{
    if (_clientsMap.size() == _modes.limit)
    {
        return false;
    }
    else if (_clientsMap.find(client.getId()) != _clientsMap.end())
    {
        return false;
    }
    else
    {
        _clientsMap.insert(std::make_pair(client.getId(), &client));
        return true;
    }

}

bool    Channel::isClientInChannel(int fd) const
{
    return (_clientsMap.find(fd) == _clientsMap.end()) ? false : true;
}

void Channel::printList(int fd) const
{
    std::string nameList;

    for (ClientMap::const_iterator it = _clientsMap.begin(); it != _clientsMap.end(); it++)
    {
        nameList += it->second->getNickname() + "\n";
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
    *this = other;
}

Channel::modes& Channel::modes::operator=(const modes &other)
{
    if (this != &other)
    {
        invite_only = other.invite_only;
        topic_restricted = other.topic_restricted;
        key = other.key;
        operator_privs = other.operator_privs;
        limit = other.limit;
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

bool    Channel::isOperator(const int fd) const
{
    return (_operators.find(fd) != _operators.end());
}

bool    Channel::isInChannel(const int fd) const
{
    return (_clientsMap.find(fd) != _clientsMap.end());
}

std::string Channel::getTopic(void) const
{
    return _topic;
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

void Channel::deleteClient(const int fd)
{
    if (_clientsMap.erase(fd) > 0)
    {
        removeOperator(fd);
    }
}

void    Channel::addOperator(const int fd)
{
    _operators.insert(fd);
}

void    Channel::removeOperator(const int fd)
{
    _operators.erase(fd);
}

