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
#include "common.hpp"

#include <map>
#include <vector>
#include <netinet/in.h>
#include <sstream>

// Const Definitions
static const char *ADDRESS = "0.0.0.0";

// Type Definitions
const int BUFFER_SIZE = 30720;
typedef std::pair<std::string, std::string> tokenPair;
typedef std::vector<std::pair<std::string, std::string> > tokenList;

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
        std::string readMessage(int fd) const;
        int acceptNewConnection();
        /**
         *  Check in the connections vector the
         * client that have the same socket_id as fd parameter and read the fd message
        */
        void inspectEvent(int fd);
        tokenList parse(std::string buffer);
        void exec(tokenList map);


        // Callbacks
        void execJOIN(const std::string line);
        void execKICK(const std::string line);
        void execINVITE(const std::string line);
        void execTOPIC(const std::string line);
        void execMODE(const std::string line);
        void execUSER(const std::string line);
        void execPASS(const std::string line);
        void execNICK(const std::string line);

    // Data
    private:
        int _port;
        int _server_socket;
        std::string _passwd;
        struct sockaddr_in _socket_addr;
        std::vector<Client *> _connections;
        fd_set  _connections_set;
};
