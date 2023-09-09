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
#include <arpa/inet.h>
#include <fcntl.h>
#include <strings.h>

// Forward declarations
class Channel;

// Const Definitions
const int BUFFER_SIZE = 30720;

static const std::string WELCOME = "001";
static const std::string YOURHOST = "002";
static const std::string CREATED = "003";
static const std::string MYINFO = "004";
static const std::string NICKCOLLISION = "433";
static const std::string PASSMISMATCH = "464";

static const std::string NAMREPLY = "353";
static const std::string ENDOFNAMES = "366"; // <channel> :<info>
static const std::string CHANNELMODEIS = "324"; // <channel> <mode> <mode_params>
static const std::string CREATIONTIME = "329";
static const std::string WHOSPCRPL = "354";
static const std::string ENDOFWHO = "315";
// Error codes
static const std::string NICKNOTFOUND = "401";
static const std::string BADJOIN = "448";

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
    void                            execUSER(Client& client, const std::string line);
    void                            execNICK(Client& client, const std::string line);
    void                            execLIST(Client& client, const std::string line);
    void                            execWHO(Client& client, const std::string line);
    void                            execQUIT(Client& client, const std::string line);
    void                            execPRIVMSG(Client& client, const std::string line);
    // Channel-specific commands
    void                            execJOIN(Client& client, const std::string line);
    void                            execKICK(Client& client, const std::string line);
    void                            execINVITE(Client& client, const std::string line);
    void                            execTOPIC(Client& client, const std::string line);
    void                            execMODE(Client& client, const std::string line);
    void                            execPART(Client& client, const std::string line);

    // replyMessages.cpp
    void                            replyPassMissMatch(const Client& client) const;
    void                            replyWelcome(const Client& client) const;
    void                            replyYourHost(const Client& client) const;
    void                            replyCreated(const Client& client) const;
    void                            replyMyInfo(const Client& client) const;
    void                            replyNickCollision(const Client& client) const;
    void                            replyPrivMessageNickNotFound(const Client& client,  const std::string &targetNickName) const;
    void                            replyPrivateMessage(const Client& client,  const Client& targetCLient, const std::string message) const;
    void                            replyJoin(const Client& client, const Channel& channel) const;
    void                            replyName(const Client& client, const Channel& channel) const;
    void                            replyEndOfNames(const Client& client, const Channel& channel) const;
    void                            replyChannelMode(const Client& client, const Channel& channel) const;
    void                            replyCreationTime(const Client& client, const Channel& channel) const;
    void                            replyWho(const Client& client, const Channel& channel) const;
    void                            replyEndOfWho(const Client& client, const Channel& channel) const;
    void                            replyBadJoin(const Client& client, const std::string& line) const;

    // clientManager.cpp
    ConnectionsList::iterator       getClient(const int fd);
    ConnectionsList::iterator       getClient(const std::string &nickname);
    void                            deleteClient(const int fd);
    // Auxiliary functions
    static void                     printList(const ConnectionsList& list, const int fd);
    //Channel related
    ChannelsList::const_iterator    getChannel(const std::string& name)const;
    static const std::string        returnChannelName(const std::string& line);


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
