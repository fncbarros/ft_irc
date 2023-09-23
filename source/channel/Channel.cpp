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

bool Channel::addClient(const Client& client, const bool chanop)
{
    if ((_modes.limit != 0) && (_clientsMap.size() == _modes.limit))
    {
        // TODO: REPLY >> Cannot join #testingIRCforProjectPurposes (User limit reached)
        return false;
    }
    else if (_clientsMap.find(client.getId()) != _clientsMap.end())
    {
        return false;
    }
    else
    {
        _clientsMap.insert(std::make_pair(client.getId(), &client));
        if (chanop)
            addOperator(client.getId());
        return true;
    }

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


std::string Channel::getTopic(void) const
{
    return _topic;
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
    if (_clientsMap.erase(fd) > 0)
    {
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

bool Channel::authenticate(const std::string& key)
{
    return key == _modes.key;
}
