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

#include <Client.hpp>
class Client;

// Type definitions
typedef bool operator_t;
typedef std::pair<const Client*, operator_t> User;
typedef std::vector<User> UserList;

class Channel
{
public:
    // Special functions
    Channel (const std::string name, const Client& client);
    ~Channel ();

    // Getters
    std::string getName() const;
    UserList    getList() const;

    void        addClient(const Client& client);
    void        printList(int fd) const;


    // Data
private:
    std::string _name;
    std::string _topic;
    UserList    _users;
};
