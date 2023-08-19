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
        bool                isOperator() const;

    // Data
    private:
        int         _socket_id;
        std::string _nickname;
        std::string _username;
        bool        _operator;
};
