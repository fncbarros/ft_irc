/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshintak <bshintak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 14:46:56 by falmeida          #+#    #+#             */
/*   Updated: 2023/09/23 14:32:23 by bshintak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <common.hpp>
#include <set>

// Forward declarations
class Channel;

// Type definitions
typedef struct auth {
    bool    isValidPassword;
    bool    isValidUser;
    bool    isValidNickName;

    auth();
    auth(const auth& other);
    auth operator=(const auth& other);

} t_auth;

class Client
{
public:
    Client(const int socket_id);
    Client(const Client& other);
    ~Client();
    Client&         operator=(const Client& other);
    int             operator==(const Client& other);
    std::string     toString(void) const;

    std::string     getNickname(void) const;
    std::string     getUsername(void) const;
    bool            isValid(void) const;
    bool            isPassActive(void) const;
    bool            isUserActive(void) const;
    bool            isNickActive(void) const;
    bool            wasInvited(const std::string& channel) const;
    int             getId(void) const;
    bool            isOperator(void) const;
    bool            isMessageWaiting(void) const;
    void            setUsername(const std::string& name);
    void            setNickname(const std::string& name);
    void            setPassActive(void);
    void            setUserActive(void);
    void            setNickActive(void);
    void            setInvited(const std::string& channel);
    void            removeInvited(const std::string& channel);
    void            registerBuffer(const std::string msg);
    void            setMessage(const std::string& msg);
    void            replyMessage(void);
    std::string     returnLine(void);

private:
    int                         _socket_id;
    std::string                 _nickname;
    std::string                 _username;
    t_auth                      _auth;
    std::string                 _msgBuffer;
    std::vector<std::string>    _replyMessages;
    std::set<std::string>       _invitedTo;
};
