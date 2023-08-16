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
    if (_socket_id)
        close(_socket_id);
    std::cout << "Client destroyed" << std::endl;
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

void Client::execJOIN(const std::string line)
{
    std::cout << "***JOIN: ";
    std::cout << line << std::endl;
}

void Client::execKICK(const std::string line)
{
    std::cout << "***KICK: ";
    std::cout << line << std::endl;
}

void Client::execINVITE(const std::string line)
{
    std::cout << "***INVITE: ";
    std::cout << line << std::endl;
}

void Client::execTOPIC(const std::string line)
{
    std::cout << "***TOPIC: ";
    std::cout << line << std::endl;
}

void Client::execMODE(const std::string line)
{
    std::cout << "***MODE: ";
    std::cout << line << std::endl;
}

void Client::execUSER(const std::string line)
{
    std::cout << "***USER: ";
    std::cout << line << std::endl;
}

void Client::execPASS(const std::string line)
{
    std::cout << "***PASS: ";
    std::cout << line << std::endl;
}

void Client::execNICK(const std::string line)
{
    std::cout << "***NICK: ";
    std::cout << line << std::endl;
}

void Client::execLIST()
{
    std::cout << "***LIST***\n";
}

void Client::execWHO()
{
    std::cout << "***WHO***\n";
}

void Client::execQUIT()
{
    std::cout << "***QUIT***\n";
}

void Client::execPRIVMSG(const std::string line)
{
    std::cout << "***PRIVMSG: ";
    std::cout << line << std::endl;
}
