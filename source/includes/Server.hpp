/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshintak <bshintak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 19:12:25 by fbarros           #+#    #+#             */
/*   Updated: 2023/09/06 18:51:48 by bshintak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Client.hpp"
#include "common.hpp"

#include <map>
#include <vector>
#include <netinet/in.h>
#include <sstream>
#include <arpa/inet.h>
#include <fcntl.h>
#include <strings.h>
#include <cstring>


// Const Definitions
const int BUFFER_SIZE = 30720;

static const std::string PASSMISMATCH = "464";
static const std::string WELCOME = "001";
static const std::string YOURHOST = "002";
static const std::string CREATED = "003";
static const std::string MYINFO = "004";
static const std::string NICKCOLLISION = "433";
static const std::string NICKNOTFOUND = "401";
// static const char *ADDRESS = "0.0.0.0";

// Type Definitions
typedef std::pair<std::string, std::string> tokenPair;
typedef std::vector<tokenPair> tokenList;
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

    // parser.cpp
    tokenList                       parse(std::string buffer);
    std::string                     getToken(const std::string token, tokenList processedMsg);



    // authentication.cpp
    bool                            auth(Client& client, const tokenPair& processedMsg);
    bool                            checkPassword(Client& client, const tokenPair& processedMsg);
    void                            checkUser(Client& client, tokenPair processedMsg);
    void                            activateClient(Client& client);


    // exec.cpp
    void                            exec(Client& client, const tokenPair& message);
    void                            execKICK(Client& client, const std::string line);
    void                            execINVITE(Client& client, const std::string line);
    void                            execTOPIC(Client& client, const std::string line);
    void                            execMODE(Client& client, const std::string line);
    void                            execUSER(Client& client, const std::string line);
    void                            execNICK(Client& client, const std::string line);
    void                            execJOIN(Client& client, const std::string line);
    void                            execLIST(Client& client, const std::string line);
    void                            execWHO(Client& client, const std::string line);
    void                            execQUIT(Client& client, const std::string line);
    void                            execPRIVMSG(Client& client, const std::string line);

    // replyMessages.cpp
    void                            replyPassMissMatch(Client& client) const;
    void                            replyWelcome(Client& client) const;
    void                            replyYourHost(Client& client) const;
    void                            replyCreated(Client& client) const;
    void                            replyMyInfo(Client& client) const;
    void                            replyNickCollision(Client& client) const;
    void                            replyPrivMessageNickNotFound(Client& client,  const std::string &targetNickName) const;
    void                            replyPrivateMessage(Client& client,  Client& targetCLient, const std::string message);
    void                            replyCAPLS(Client& client, std::string capabilities) const;
    // clientManager.cpp
    ConnectionsList::iterator       getClient(const int fd);
    ConnectionsList::iterator       getClient(const std::string &nickname);
    void                            deleteClient(const int fd);
    // Auxiliary functions
    static void                     printList(const ConnectionsList& list, const int fd);
    static void                     printList(const ClientList& list, const int fd);

public:
    void                            connectionLoop(void);
    int                             setConnection(const int port, const std::string password);
    void                            setPassword(const std::string password);
    void                            setCurrentDate(void);
    void                            interrupt(void);

    // Data
private:
    int                     _server_socket;
    std::string             _password;
    struct sockaddr_in      _socket_addr;
    fd_set                  _connections_set;
    ConnectionsList         _connections;
    ChannelsList            _channels;
    bool                    _interrupt;
    std::string             _server_date_created;

};
