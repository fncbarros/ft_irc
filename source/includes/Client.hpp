/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falmeida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 14:46:56 by falmeida          #+#    #+#             */
/*   Updated: 2023/08/12 14:47:00 by falmeida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <common.hpp>

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
    // Special functions
public:
    Client(const int socket_id);
    Client(const Client& other);
    ~Client();
    Client&         operator=(const Client& other);
    int             operator==(const Client& other);

    // Public functions
    std::string     getNickname(void) const;
    std::string     getUsername(void) const;
    bool            isValid(void) const;
    bool            isPassActive(void) const;
    bool            isUserActive(void) const;
    bool            isNickActive(void) const;
    int             getId(void) const;
    bool            isOperator(void) const;
    bool            isMessageWaiting(void) const;
    void            setUsername(const std::string& name);
    void            setNickname(const std::string& name);
    void            setPassActive(void);
    void            setUserActive(void);
    void            setNickActive(void);
    void            registerBuffer(const std::string msg);
    std::string     returnLine(void);

    // Data
private:
    int                     _socket_id;
    std::string             _nickname;
    std::string             _username;
    t_auth                  _auth;
    std::string             _msgBuffer;
};
