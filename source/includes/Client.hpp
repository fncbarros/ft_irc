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

#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "common.hpp"

class Client
{
    // Special functions
    public:
        Client(const std::string &nick, const std::string &user, const std::string &pass);
        ~Client();
    
    // Public functions
    public:
        const std::string getNickname(void) const;
        const std::string getUsername(void) const;
        const std::string getPassword(void) const;
        bool        auth(const std::string &nick, const std::string &pass) const;

    // Internal functions
    private:
        Client();


    // Data
    private:
        std::string _nickname;
        std::string _username;
        std::string _password;
};

#endif
