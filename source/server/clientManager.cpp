/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falmeida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 14:47:11 by falmeida          #+#    #+#             */
/*   Updated: 2023/08/12 14:47:13 by falmeida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

ConnectionsList::iterator Server::getClient(const int fd)
{
    for (ConnectionsList::iterator client = _connections.begin(); client != _connections.end(); client++)
    {
        if (client->getId() == fd)
            return client;
    }
    return _connections.end();
}

ConnectionsList::iterator Server::getClient(const std::string &nickname)
{
    for (ConnectionsList::iterator client = _connections.begin(); client != _connections.end(); client++)
    {
        if (client->getNickname() == nickname)
            return client;
    }
    return _connections.end();
}

void Server::deleteClient(const int fd)
{
    ConnectionsList::iterator client = getClient(fd);
    if (client != _connections.end())
        _connections.erase(client);
    close(fd);
    FD_CLR(fd, &_connections_set);
    std::cout << "Client deleted" << std::endl;
}

// Static functions
void Server::printList(const ConnectionsList& list, const int fd)
{
    std::string nameList;
    for (ConnectionsList::const_iterator it = list.begin(); it != list.end(); it++)
    {
        nameList += it->getNickname() + "\n";
    }
    Utils::writeTo(nameList, fd);
}
