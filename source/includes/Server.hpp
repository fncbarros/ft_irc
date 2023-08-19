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
// static const char *ADDRESS = "0.0.0.0";

// Type Definitions
const int BUFFER_SIZE = 30720;
typedef std::pair<std::string, std::string> tokenPair;
typedef std::vector<std::pair<std::string, std::string> > tokenList;

class Server
{
    // Special functions
    public:
         Server();
        ~Server();

    // Public functions
    public:
        void        connectionLoop(void);
        int         setConnection(const int port, const std::string password);
        void        interrupt();

    // Internal functions
    private:
        std::string readMessage(int fd) const;
        int         acceptNewConnection();
        /**
         *  Check in the connections vector the
         * client that have the same socket_id as fd parameter and read the fd message
        */
        void        inspectEvent(int fd);
        tokenList   parse(std::string buffer);
        void        validateToken(std::string& token) const;
        void        exec(Client& client, tokenList processedMsg);
        bool        auth( const std::string &password) const;

    public:
        // Operation methods
        void        execJOIN(Client& client, const std::string line);
        void        execKICK(Client& client, const std::string line);
        void        execINVITE(Client& client, const std::string line);
        void        execTOPIC(Client& client, const std::string line);
        void        execMODE(Client& client, const std::string line);
        void        execUSER(Client& client, const std::string line);
        void        execPASS(Client& client, const std::string line);
        void        execNICK(Client& client, const std::string line);
        void        execLIST(Client& client, const std::string line);
        void        execWHO(Client& client, const std::string line);
        void        execQUIT(Client& client, const std::string line);
        void        execPRIVMSG(Client& client, const std::string line);

    // Data
    private:
        int                     _server_socket;
        std::string             _password;
        struct sockaddr_in      _socket_addr;
        std::vector<Client *>   _connections;
        bool                    _interrupt;
        fd_set                  _connections_set;
};
