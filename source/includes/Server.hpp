/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbarros <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 19:12:25 by fbarros           #+#    #+#             */
/*   Updated: 2023/08/09 19:13:22 by fbarros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Client.hpp"

#include <string>
#include <map>
#include <vector>
#include <netinet/in.h>
#include <sstream>

// Type Definitions
typedef std::pair<std::string, std::string> Operation;
typedef std::map<std::string, std::string> OperationsMap;

// Const Definitions
static const unsigned int SOCKLEN = sizeof(sockaddr_in);
static const std::string c_tokensArray[] =
{
    "USER",
    "NICK",
    "PASSWORD",
    // etc.
};
const Operation c_operationsPairArray[] = {
    std::make_pair("JOIN", "join"),
    std::make_pair("KICK", "kick"),
    std::make_pair("INVITE", "invite"),
    std::make_pair("TOPIC", "topic"),
    std::make_pair("MODE", "mode"),
};
static const OperationsMap c_operationsMap(c_operationsPairArray, c_operationsPairArray + sizeof(c_operationsPairArray) / sizeof(c_operationsPairArray[0]));

class Server
{
    // Special functions
    public:
        Server(int port, std::string passwd);
        ~Server();

    // Public functions
    public:
        void connectionLoop(void);

    // Internal functions
    private:
        Server(); // no public default constructor
        void setConnection();
        std::string readMessage(void) const;
        void parse(std::string buffer);

    // Data
    private:
        int _port;
        int _socket;
        int _socket_process;
        std::string _passwd;
        struct sockaddr_in _socket_addr;
        std::vector<Client> _connections;

};
