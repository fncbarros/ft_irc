/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbarros <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 19:22:13 by fbarros           #+#    #+#             */
/*   Updated: 2023/08/09 19:22:16 by fbarros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Client.hpp"

// Type definitions
// TODO: cannot use ref and need "class" qualifier for some reason
typedef std::vector<const class Client *> ClientList;

class Channel
{
public:
    // Special functions
    Channel (const std::string name, const Client& client);
    ~Channel ();

    // Getters
    std::string getName() const;
    ClientList  getList() const;

    void        addClient(const Client& client);

    // Data
private:
    std::string _name;
    ClientList  _clientList;
};
