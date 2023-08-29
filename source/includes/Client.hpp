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

#include "common.hpp"

typedef struct auth {
    bool    isValidPassword;
    bool    isValidUser;
    bool    isValidNickName;
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
        void            setUsername(const std::string& name);
        void            setNickname(const std::string& name);
        void            setPassActive(void);
        void            setUserActive(void);
        void            setNickActive(void);
        bool            isOperator(void) const;

        // Data
    private:
        int         _socket_id;
        std::string _nickname;
        std::string _username;
        bool        _operator;
        t_auth      _auth;

};
