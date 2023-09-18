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
#include <set>

class Client;

// Type definitions
typedef std::map<int, const Client*> ClientMap;

class Channel
{
public:
    // Special functions
    Channel();
    Channel(const std::string name);
    Channel(const Channel& other);
    Channel& operator=(const Channel& other);
    ~Channel();

    // modes type
    struct modes {
        bool        invite_only;
        bool        topic_restricted;
        std::string key;
        bool        operator_privs;
        size_t      limit;

        modes();
        modes(const modes& other);
        modes& operator=(const modes& other);
    };

    // Getters
    std::string         getName() const;
    const ClientMap&    getClients() const;

    bool                addClient(const Client& client, const bool chanop = false);
    bool                isClientInChannel(int fd) const;
    void                printList(int fd) const; // DEBUG

    bool                isInviteOnly(void) const;
    bool                isTopicRetricted(void) const;
    bool                hasKey(void) const;
    bool                hasOperatorPriviledges(void) const;
    size_t              limit(void) const;
    bool                hasModes(void) const;
    bool                isOperator(const int fd) const;
    bool                isInChannel(const int fd) const;

    std::string         getTopic(void) const;

    void                setInviteOnly(const bool set);
    void                setTopicRestriction(const bool set);
    void                setKey(const std::string& key);
    void                setNoKey(void);
    void                setPriviledges(const bool set);
    void                setLimit(const size_t limit);
    void                deleteClient(const int fd);
    void                addOperator(const int fd);
    void                removeOperator(const int fd);

    // Data
private:
    std::string     _name;
    std::string     _topic;
    struct modes    _modes;
    ClientMap       _clientsMap;
    std::set<int>   _operators;

};
