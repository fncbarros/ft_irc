/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falmeida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 14:47:11 by falmeida          #+#    #+#             */
/*   Updated: 2023/08/12 14:47:13 by falmeida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Client.hpp>

Client::Client(const int socket_id)
: _socket_id(socket_id)
, _operator(false)
{
    std::cout << "Client created: " << _socket_id << std::endl;
}

Client::Client(const Client& other)
: _auth(other._auth)
{
    _nickname = other.getNickname();
    _username = other.getUsername();
    _socket_id = other.getId();
    _operator = other.isOperator();
}

Client& Client::operator=(const Client& other)
{
    if (this != &other)
        *this = other;
    return *this;
}

Client::~Client()
{
}

int Client::operator==(const Client& other)
{
    return (_socket_id == other._socket_id) && (_username == other._username);
}

// Getters
std::string Client::getNickname(void) const { return _nickname; }

std::string Client::getUsername(void) const { return _username; }

int Client::getId(void) const { return _socket_id; }

bool  Client::isPassActive(void) const { return _auth.isValidPassword; }

bool  Client::isUserActive(void) const { return !_username.empty(); }

bool  Client::isNickActive(void) const { return !_nickname.empty(); }

bool  Client::isValid(void) const { return (_auth.isValidPassword && _auth.isValidNickName && _auth.isValidUser); }

bool Client::isOperator(void) const { return _operator; }

bool Client::isMessageWaiting(void) const { return (_msgBuffer.find("\r\n") != std::string::npos); }

void Client::setUsername(const std::string& name) { _username = name; }

void Client::setNickname(const std::string& name) { _nickname = name; }

void Client::setPassActive(void) { _auth.isValidPassword = true; }

void Client::setUserActive(void) { _auth.isValidUser = true; }

void Client::setNickActive(void) { _auth.isValidNickName = true; }

void Client::registerBuffer(const std::string msg)
{
    if (msg.empty())
        return ;
    _msgBuffer.append(msg);
}

std::string Client::returnLine(void)
{
    const size_t pos(_msgBuffer.find("\r\n"));

    if (pos == std::string::npos)
        return "";

    const std::string s(_msgBuffer.substr(0u, pos));
    _msgBuffer.erase(0u, pos + 1);

    return s;
}
