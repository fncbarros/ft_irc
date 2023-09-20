/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshintak <bshintak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 19:12:25 by fbarros           #+#    #+#             */
/*   Updated: 2023/09/20 16:21:00 by bshintak         ###   ########.fr       */
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
const int BUFFER_SIZE(30720);
const std::string EOL("\r\n");

static const std::string WELCOME("001");
static const std::string YOURHOST("002");
static const std::string CREATED("003");
static const std::string MYINFO("004");
static const std::string NICKCOLLISION("433");
static const std::string PASSMISMATCH("464");

static const std::string NAMREPLY("353");
static const std::string ENDOFNAMES("366"); // <channel> :<info>
static const std::string CHANNELMODEIS("324"); // <channel> <mode> <mode_params>
static const std::string CREATIONTIME("329");
static const std::string WHOSPCRPL("354");
static const std::string ENDOFWHO("315");
static const std::string LISTSTART("321");
static const std::string LIST("322");
static const std::string LISTEND("323");
static const std::string INVITING("341");

// Error codes
static const std::string NICKNOTFOUND("401");
static const std::string NOSUCHNICK("401");
static const std::string CHANNELNOTFOUND("403");
static const std::string NEEDMOREPARAMS("461");
static const std::string CLIENTONCHANNEL("443");
static const std::string BADJOIN("448");
static const std::string NOSUCHCHANNEL("403");
static const std::string USERNOTINCHANNEL("441");
static const std::string CLIENTNOTONCHANNEL("441");
static const std::string CHANOPRIVSNEEDED("482");
static const std::string KICK("312");
static const std::string NOTONCHANNEL("442");


// Type Definitions
typedef std::pair<std::string, std::string> tokenPair;
typedef std::vector<tokenPair> tokenList;
typedef std::vector<Client> ConnectionsList;
typedef std::vector<Channel> ChannelsList;

class Server
{
    typedef void (Server::*exec_ptr)(Client&, const std::string);
    typedef std::map<std::string, exec_ptr>  CommandMap;

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
public:
    void                            exec(Client& client, const tokenPair& message);
    void                            execUSER(Client& client, const std::string line);
    void                            execNICK(Client& client, const std::string line);
    void                            execLIST(Client& client, const std::string line);
    void                            execWHO(Client& client, const std::string line);
    void                            execQUIT(Client& client, const std::string line);
    void                            execPRIVMSG(Client& client, const std::string line);
    // Channel-specific commands
    void                            execCAP(Client& client, std::string command);
    void                            execJOIN(Client& client, const std::string line);
    void                            execKICK(Client& client, const std::string line);
    void                            execINVITE(Client& client, const std::string line);
    void                            execTOPIC(Client& client, const std::string line);
    void                            execMODE(Client& client, const std::string line);
    void                            execPART(Client& client, const std::string line);

private:
    // replyMessages.cpp
    void                            addMessage(const std::string& message, int fd);
    void                            replyPassMissMatch(const Client& client);
    void                            replyWelcome(const Client& client);
    void                            replyYourHost(const Client& client);
    void                            replyCreated(const Client& client);
    void                            replyMyInfo(const Client& client);
    void                            replyNickCollision(const Client& client);
    void                            replyPrivMessageNickNotFound(const Client& client, const std::string& targetNickName);
    void                            replyPrivateMessage(const Client& client,  const Client& targetCLient, const std::string& message);
    void                            replyName(const Client& client, const Channel& channel);
    void                            replyChannelNotFound(const Client& client, const std::string& channelName);
    void                            replyChannelMessage(const Client& client,  const Client& clientSender, const std::string& channelName, const std::string& message);
    void                            replyCAPLS(Client& client, std::string capabilities);
    void                            replyJoin(const Client& client, const Channel& channel);
    void                            replyEndOfNames(const Client& client, const Channel& channel);
    void                            replyChannelMode(const Client& client, const Channel& channel);
    void                            replyCreationTime(const Client& client, const Channel& channel);
    void                            replyWho(const Client& client, const Channel& channel);
    void                            replyEndOfWho(const Client& client, const Channel& channel);
    void                            replyBadJoin(const Client& client, const std::string& line);
    void                            replyList(const Client& client);
    void                            replyList(const Client& client, const Channel& channel);
    void                            replyNoSuchNickError(const Client& client, const std::string& nickTarget);
    void                            replyNotOnChannelError(const Client& client, const std::string& channelName);
    void                            replyClientTargetOnChannel(const Client& client, const std::string& nickTarget, const std::string& channelName);
    void                            replyInviting(const Client& client, const std::string& nickTarget, const std::string& channelName);
    void                            replyInvitingReceived(const Client& client, const Client& clientTarget, const std::string& channelName);
    void                            replyNoSuchChannel(const Client& client);
    void                            replyNoSuchNick(const Client& client, const std::string& str);
    void                            replyNotInChannel(const Client& client, const std::string& userNick, const std::string& channelName);
    void                            replyNoPriviledges(const Client& client, const std::string& channelName);
    void                            replyKick(const Client& client, const Client& kicker, const Channel& channel, const std::string& userNick, const std::string& reason);
    void                            replyBroadcastKick(const int id, const std::string& kickerNick, const std::string& userNick, const std::string& channelName, const std::string& reason);
    void                            replyNotOnChannel(const Client& client, const std::string& channelName);
    void                            replyPart(const Client& client, const std::string& channelName);
    void                            replyYouWereKicked(const int id, const std::string& channelName, const std::string& kickerNick, const std::string& reason);
    void                            replyPartUsage(const int id);
    void                            replyNoSuchChannelSimple(const int id, const std::string& channelName);
    void                            replyYouLeftChannel(const int id, const std::string& channelName, const std::string& reason);
    void                            replyBroadcastUserLeft(const int id, const Client& client, const std::string& reason);
    void                            replyTopic(const Client& client, const std::string& channelTopic);
    void                            replyNeedMoreParams(const Client& client);


    // communication.cpp
    void                            channelPrivateMessage(const Client& client, const std::string& channelname, const std::string& message);
    void                            clientPrivateMessage(const Client& client, const std::string& nickname, const std::string& message);

    // clientManager.cpp
    ConnectionsList::iterator       getClient(const int fd);
    ConnectionsList::const_iterator getClient(const int fd) const;
    ConnectionsList::const_iterator getClient(const std::string &nickname) const;
    void                            deleteClient(const int fd);
    // Auxiliary functions
    static void                     printList(const ConnectionsList& list, const int fd);
    //Channel related
    ChannelsList::iterator          getChannel(const std::string& name);
    ChannelsList::const_iterator    getChannel(const std::string& name) const;
    static const std::string        returnChannelName(const std::string& line);
    bool                            channelExists(const std::string& name) const;

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
    fd_set                  _connections_set_read;
    fd_set                  _connections_set_write;
    ConnectionsList         _connections;
    ChannelsList            _channels;
    bool                    _interrupt;
    std::string             _server_date_created;
    CommandMap              _commands;

};
