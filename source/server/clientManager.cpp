#include "../includes/Server.hpp"

ConnectionsList::iterator Server::getClient(const int fd)
{
    for (ConnectionsList::iterator client = _connections.begin(); client != _connections.end(); client++)
    {
        if (client->getId() == fd)
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

void Server::printClientList(const ClientList& list, const int fd)
{
    std::string nameList;
    for (ClientList::const_iterator it = list.begin(); it != list.end(); it++)
    {
        nameList += (*it)->getNickname() + "\n";
    }
    Utils::writeTo(nameList, fd);
}