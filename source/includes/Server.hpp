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
typedef void (*OperationCallback)(const std::string);
typedef std::pair<std::string, OperationCallback> Operation;
typedef std::map<std::string, OperationCallback> OperationsMap;

class Server
{
    // Const Data
    private:
        const Operation c_operationsPairArray[9];
        const OperationsMap c_operationsMap;

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
        std::string readMessage(int fd) const;
        int acceptNewConnection();
        /**
         *  Check in the connections vector the 
         * client that have the same socket_id as fd parameter and read the fd message
        */
        void inspectEvent(int fd); 
        void parse(std::string buffer);

        // Callbacks
        static void execJOIN(const std::string line);
        static void execKICK(const std::string line);
        static void execINVITE(const std::string line);
        static void execTOPIC(const std::string line);
        static void execMODE(const std::string line);
        static void execUSER(const std::string line);
        static void execPASS(const std::string line);
        static void execNICK(const std::string line);

    // Data
    private:
        int _port;
        int _server_socket;
        std::string _passwd;
        struct sockaddr_in _socket_addr;
        std::vector<Client> _connections;
        fd_set  _connections_set;
};
