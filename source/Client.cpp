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

Client::Client(const int socket_id)
: _socket_id(socket_id)
{
    std::cout << "Client created: " << _socket_id << std::endl;
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
int         Client::getId(void)       const { return (_socket_id); }

bool        Client::auth(const std::string &nick, const std::string &pass) const
{
    return (nick == _nickname) && (pass == _password);
}

bool        Client::isOperator() const
{
    return _operator;
}