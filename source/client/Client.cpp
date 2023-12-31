/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshintak <bshintak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 14:47:11 by falmeida          #+#    #+#             */
/*   Updated: 2023/09/23 14:15:47 by bshintak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Client.hpp>

Client::Client(const int socket_id)
: _socket_id(socket_id)
{
    std::cout << "Client created: " << _socket_id << std::endl;
}

Client::Client(const Client& other)
: _auth(other._auth)
{
    *this = other;
}

Client& Client::operator=(const Client& other)
{
    if (this != &other)
    {
        _nickname = other.getNickname();
        _username = other.getUsername();
        _socket_id = other.getId();
    }
    return *this;
}

Client::~Client()
{
}

int Client::operator==(const Client& other)
{
    return (_socket_id == other._socket_id) && (_username == other._username);
}

std::string     Client::toString(void) const
{
    return (_nickname + "!" + _username + "@" + HOST);
}


// Getters
std::string Client::getNickname(void) const { return _nickname; }

std::string Client::getUsername(void) const { return _username; }

int Client::getId(void) const { return _socket_id; }

bool  Client::isPassActive(void) const { return _auth.isValidPassword; }

bool  Client::isUserActive(void) const { return _auth.isValidUser; }

bool  Client::isNickActive(void) const { return _auth.isValidNickName; }

bool Client::wasInvited(const std::string& channel) const{ return _invitedTo.find(channel) != _invitedTo.end(); }

bool  Client::isValid(void) const { return (_auth.isValidPassword && _auth.isValidNickName && _auth.isValidUser); }

bool Client::isMessageWaiting(void) const { return (_msgBuffer.find(EOL) != std::string::npos); }

void Client::setUsername(const std::string& name) { _username = name; }

void Client::setNickname(const std::string& name) { _nickname = name; }

void Client::setPassActive(void) { _auth.isValidPassword = true; }

void Client::setUserActive(void) { _auth.isValidUser = true; }

void Client::setNickActive(void) { _auth.isValidNickName = true; }

void Client::setInvited(const std::string& channel) { _invitedTo.insert(channel); }

void Client::removeInvited(const std::string& channel) { _invitedTo.erase(channel); }

void Client::setMessage(const std::string& msg) { _replyMessages.push_back(msg); }

void Client::registerBuffer(const std::string msg)
{
    if (msg.empty())
        return ;
    _msgBuffer.append(msg);
}

std::string Client::returnLine(void)
{
    const size_t pos(_msgBuffer.find(EOL));

    if (pos == std::string::npos)
        return "";

    const std::string s(_msgBuffer.substr(0u, pos));
    _msgBuffer.erase(0u, pos + 1u);

    return s;
}

void    Client::replyMessage(void)
{
    if (!_replyMessages.empty())
    {
        Utils::writeTo(_replyMessages.begin()->c_str(), _socket_id);
        _replyMessages.erase(_replyMessages.begin());
    }
}

t_auth::auth()
: isValidPassword(false)
, isValidUser(false)
, isValidNickName(false)
{
}

t_auth::auth(const auth& other)
{
    isValidPassword = other.isValidPassword;
    isValidUser = other.isValidUser;
    isValidNickName = other.isValidNickName;
}

auth t_auth::operator=(const auth& other)
{
    if (this != &other)
    {
        *this = other;
    }
    return *this;
}

void Client::addToBuffer(const std::string msg)
{
    _buffer += msg;
}

std::string Client::getLine(void)
{
    size_t pos(_buffer.rfind('\n'));
    if (pos == std::string::npos)
        return "";
    const std::string line(_buffer.substr(0u, pos + 1));
    _buffer.erase(0u, pos + 1);
    return line;
}
