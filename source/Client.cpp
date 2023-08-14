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

#include "includes/Client.hpp"

#include <iostream>

Client::Client(const std::string &nick, const std::string &user, const std::string &pass)
: _nickname(nick)
, _username(user)
, _password(pass)
{
    std::cout << "Client created: " << _nickname << " " << _username << " " << _password << std::endl;
}

Client::~Client()
{
    std::cout << "Client destroyed" << std::endl;
}

Client::Client(/* args */)
{
}

// Public functions
const std::string Client::getNickname(void) const { return (_nickname); }
const std::string Client::getUsername(void) const { return (_username); }
const std::string Client::getPassword(void) const { return (_password); }

bool        Client::auth(const std::string &nick, const std::string &pass) const
{
    return (nick == _nickname) && (pass == _password);
}

bool        Client::isOperator() const
{
    return _operator;
}