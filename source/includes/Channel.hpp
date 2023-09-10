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
typedef std::pair<const Client*, bool> User; // true if operator
typedef std::vector<User> UserList;

class Channel
{
public:
    // Special functions
    Channel();
    Channel (const std::string name, const Client& client);
    Channel(const Channel& other);
    Channel& operator=(const Channel& other);
    ~Channel ();

    // Getters
    std::string getName() const;
    UserList    getList() const;

    bool        addClient(const Client& client);
    void        printList(int fd) const;

    struct modes {
        bool invite_only;
        bool topic_restricted;
        std::pair<bool, std::string> key;
        bool operator_privs;
        size_t limit;

        modes();
        modes(const modes& other);
        modes& operator=(const modes& other);
    };

    bool        isInviteOnly(void) const;
    bool        isTopicRetricted(void) const;
    bool        hasKey(void) const;
    bool        hasOperatorPriviledges(void) const;
    size_t      limit(void) const;
    bool        hasModes(void) const;

    std::string getTopic(void) const;

    void        setInviteOnly(const bool set);
    void        setTopicRestriction(const bool set);
    void        setKey(const std::string& key);
    void        setNoKey(void);
    void        setPriviledges(const bool set);
    void        setLimit(const size_t limit);

    // Data
private:
    std::string     _name;
    std::string     _topic;
    struct modes    _modes;
    UserList        _users;
};
