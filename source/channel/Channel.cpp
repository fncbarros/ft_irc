/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshintak <bshintak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 19:12:25 by fbarros           #+#    #+#             */
/*   Updated: 2023/09/23 16:32:11 by bshintak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Channel.hpp>

Channel::Channel()
{
    // ...
}

Channel::Channel(const std::string name)
: _name(name)
{
    std::cout << "Channel " << name << " created." << std::endl;
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
}

std::string Channel::getName(void) const
{
    return _name;
}

std::string Channel::getTopic(void) const
{
    return _topic;
}

int Channel::getTopicNick(void) const
{
    return _clientTopicNick;
}

const ClientMap& Channel::getClients(void) const
{
    return _clientsMap;
}

bool Channel::addClient(Client& client, const bool chanop)
{
    if ((limit() != 0) && (_clientsMap.size() == _modes.limit))
    {
        return false;
    }
    else if (!isInChannel(client.getId())) // if client is already in channel we ignore it
    {
        _clientsMap.insert(std::make_pair(client.getId(), &client));
        if (chanop)
            addOperator(client.getId());
        client.removeInvited(_name);
    }
    return true;
}

bool    Channel::isClientInChannel(int fd) const
{
    return (_clientsMap.find(fd) != _clientsMap.end());
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
    return !_modes.key.empty();
}

size_t Channel::limit(void) const
{
    return _modes.limit;
}

bool Channel::hasModes(void) const
{
    return (isInviteOnly() || isTopicRetricted() || hasKey() || limit());
}

bool    Channel::isOperator(const int fd) const
{
    return (_operators.find(fd) != _operators.end());
}

bool    Channel::isInChannel(const int fd) const
{
    return (_clientsMap.find(fd) != _clientsMap.end());
}

std::string Channel::returnModes(void) const
{
    std::string modes("+()");

    if (hasModes())
        modes = "+";
    if (isInviteOnly())
        modes += "i";
    if (isTopicRetricted())
        modes += "t";
    if (hasKey())
        modes += "k";
    if (limit() > 0)
        modes += "l";
    return modes;
}

void Channel::setTopic(const std::string& newTopic)
{
    _topic = newTopic;
}

void Channel::setTopicNick(int newNick)
{
    _clientTopicNick = newNick;
}

size_t Channel::size(void)const
{
    return _clientsMap.size();
}

bool Channel::setInviteOnly(const bool set)
{
    const bool changeApplied(set != isInviteOnly());
    _modes.invite_only = set;
    return changeApplied;
}

bool Channel::setTopicRestriction(const bool set)
{
    bool changeApplied(set != isTopicRetricted());
    _modes.topic_restricted = set;
    return changeApplied;
}

bool Channel::setKey(const std::string& key)
{
    const bool changeApplied(_modes.key != key);
    _modes.key = key;
    return changeApplied;
}

bool Channel::setNoKey(void)
{
    const bool changeApplied(!_modes.key.empty());
    _modes.key.clear();
    return changeApplied;
}

bool Channel::setLimit(const size_t limit)
{
    const bool changeApplied(_modes.limit != limit);
    _modes.limit = limit;
    return changeApplied;
}

bool Channel::deleteClient(const int fd)
{
    const ClientMap::iterator it(_clientsMap.find(fd));
    if (it != _clientsMap.end())
    {
        _clientsMap.erase(it);
        removeOperator(fd);
        return true;
    }
    return false;
}

bool    Channel::addOperator(const int fd)
{
    // if not already operator
    if (!isOperator(fd))
    {
        _operators.insert(fd);
        return true;
    }
    return false;
}

bool    Channel::removeOperator(const int fd)
{
    return _operators.erase(fd) > 0;
}

/**
 * modes functions
*/
Channel::modes::modes()
: invite_only(false)
, topic_restricted(false)
, key("")
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
        limit = other.limit;
    }
    return *this;
}

bool Channel::authenticate(const std::string& key)
{
    return key == _modes.key;
}
