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
typedef std::vector<Client> ConnectionsList;
typedef std::vector<Channel> ChannelsList;

class Server
{
        // Special functions
    public:
         Server();
        ~Server();

        // Internal functions
    private:
        std::string                     readMessage(int fd) const;
        int                             acceptNewConnection();
        /**
         *  Check in the connections vector the
         * client that have the same socket_id as fd parameter and read the fd message
        */
        bool                            inspectEvent(int fd);
        tokenList                       parse(std::string buffer);
        void                            validateToken(std::string& token) const;
        bool                            auth( const std::string &password) const;
        void                            deleteClient(const int fd);
        ConnectionsList::iterator       getClient(const int fd);

        // Operation methods (exec.cpp)
        void                            exec(Client& client, tokenList processedMsg);
        void                            execJOIN(Client& client, const std::string line);
        void                            execKICK(Client& client, const std::string line);
        void                            execINVITE(Client& client, const std::string line);
        void                            execTOPIC(Client& client, const std::string line);
        void                            execMODE(Client& client, const std::string line);
        void                            execUSER(Client& client, const std::string line);
        void                            execNICK(Client& client, const std::string line);
        void                            execLIST(Client& client, const std::string line);
        void                            execWHO(Client& client, const std::string line);
        void                            execQUIT(Client& client, const std::string line);
        void                            execPRIVMSG(Client& client, const std::string line);

        // Auxiliary functions
        static void                     printList(const ConnectionsList& list, const int fd);
        static void                     printList(const ClientList& list, const int fd);
//        static void                     printList(const ChannelsList& list, int fd);

    public:
        void                            connectionLoop(void);
        int                             setConnection(const int port, const std::string password);
        void                            interrupt();

        // Data
    private:
        int                     _server_socket;
        std::string             _password;
        struct sockaddr_in      _socket_addr;
        bool                    _interrupt;
        fd_set                  _connections_set;
        ConnectionsList         _connections;
        ChannelsList            _channels;
};
