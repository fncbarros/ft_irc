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

#include "../includes/Client.hpp"

Client::Client(const int socket_id)
: _socket_id(socket_id)
, _active(false)
{
    std::cout << "Client created: " << _socket_id << std::endl;
}

Client::Client(const Client& other)
{
    _nickname = other.getNickname();
    _username = other.getUsername();
    _socket_id = other.getId();
    _operator = other.isOperator();
    _active = other.isActive();
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

bool  Client::isActive(void) const { return _active; }

bool  Client::isValid(void) const { return (isActive() && !getNickname().empty() && !getUsername().empty()); }
// Setters
void Client::setUsername(const std::string name) { _username = name; }

void Client::setNickname(const std::string name) { _nickname = name; }

void Client::setActive(void) { _active = true; }


// Public functions

bool Client::isOperator() const
{
    return _operator;
}