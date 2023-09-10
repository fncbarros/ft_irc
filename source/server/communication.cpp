#include <Server.hpp>

void    Server::channelPrivateMessage(const Client& client, const std::string& channelname, const std::string& message)
{
    std::cout << "Private Message " << client.getNickname() << " user: [" << channelname << "] message: [" << message << "]" << std::endl;
    
    ChannelsList::iterator channel = getChannel(channelname);

    if (channel == _channels.end())
    {
        replyChannelNotFound(client, channelname);
    }
    else
    {
        std::cout << client.getNickname() << std::endl;
        UserList channelclients = channel->getList();
        UserList::iterator itclient;
        for (itclient = channelclients.begin(); itclient != channelclients.end(); itclient++)
        {
            if (itclient->first != &client)
            {
                replyChannelMessage(*itclient->first, client, channel->getName(), message);
            }
        }
    }
}

void    Server::clientPrivateMessage(const Client& client, const std::string& nickname, const std::string& message)
{
    ConnectionsList::iterator newClient = getClient(nickname);

    if (newClient == _connections.end())
    {
        replyPrivMessageNickNotFound(client, nickname);
    }
    else
    {
        replyPrivateMessage(client, *newClient, message);
    }
    std::cout << "Private Message " << client.getNickname() << " user: [" << nickname << "] message: [" << message << "]" << std::endl;
}
