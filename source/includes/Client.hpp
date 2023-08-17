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

class Client
{
    // Special functions
    public:
        Client(const int socket_id);
        ~Client();
    
    // Public functions
    public:
        const std::string   getNickname(void) const;
        const std::string   getUsername(void) const;
        int                 getId(void) const;
        bool                auth(const std::string &nick, const std::string &pass) const;
        bool                isOperator() const;

        // Operation methods
        void                execJOIN(const std::string line);
        void                execKICK(const std::string line);
        void                execINVITE(const std::string line);
        void                execTOPIC(const std::string line);
        void                execMODE(const std::string line);
        void                execUSER(const std::string line);
        void                execPASS(const std::string line);
        void                execNICK(const std::string line);
        void                execLIST(const std::string line);
        void                execWHO(const std::string line);
        void                execQUIT(const std::string line);
        void                execPRIVMSG(const std::string line);
    // Data
    private:
        int         _socket_id;
        std::string _nickname;
        std::string _username;
        std::string _password;
        bool        _operator;
};
