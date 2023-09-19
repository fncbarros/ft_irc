/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modes.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbarros <fbarros@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 19:22:13 by fbarros           #+#    #+#             */
/*   Updated: 2023/09/09 15:07:55 by fbarros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>
#include <cstdlib>

void Server::execMODE(Client& client, const std::string line)
{
    // TODO: might need to check for other symbols ('&','+','!')

    std::istringstream iss(line);
    std::string channelName;
    std::queue<std::string> modes;
    std::string args;

    iss >> channelName;
    if (!isChannelValid(channelName))
        return ;
    channelName.erase(0, 1);

    Channel& channel = *(getChannel(channelName));
    
    // insert all arguments in string
    while (!iss.eof())
    {
        iss >> args;
        modes.push(args);
    }

    if (!channel.isInChannel(client.getId()))
    {
        // reply
    }
    else if (!channel.isOperator(client.getId()))
    {
        // reply
    }
    else
    {
        parseModes(modes, channel, client);
    }
}

bool Server::isChannelValid(const std::string& name) const
{
    if (name.size() < 2)
    {
        // reply
    }
    else if (name[0] == '#')
    {
        // reply
    }
    else if (!channelExists(name.substr(1)))
    {
        // reply
    }
    else
    {
        return true;
    }
    return false;    
}

void Server::parseModes(std::queue<std::string>& modes, Channel& channel, const Client& client)
{
	(void)client;
    // const int id(client.getId());

    // i, t, k, o, l
    while (!modes.empty())
    {
        const std::string token = modes.front();
        modes.pop();
        bool status;

        if (token.at(0) == '+')
        {
            status = true;
        }
        else if (token.at(0) == '-')
        {
            status = false;
        }
        else
        {
            // reply
            continue ; // or return??
        }

        // TODO: replies based on result?????

        if (token.at(1) == 'i')
        {
            channel.setInviteOnly(status);
        }
        else if (token.at(1) == 't')
        {
            channel.setTopicRestriction(status);
        }
        else if (token.at(1) == 'k')
        {
            const std::string newKey(modes.front());
            modes.pop();
            status ? channel.setKey(newKey) : channel.setNoKey();
        }
        else if (token.at(1) == 'o')
        {
            const std::string user(modes.front());
            modes.pop();
			processOperator(channel, user, status);
        }
        else if (token.at(1) == 'l') // see syntax further
        {
			const std::string limit(modes.front());
			modes.pop();
			processLimit(limit, channel, status);
        }
        else // bad arg
        {
            // reply
            // return ??
        }
    }
}

void Server::processOperator(Channel& channel, const std::string& user, const bool status)
{
	ConnectionsList::const_iterator clientIt = getClient(user);
	if (clientIt == _connections.end())
	{
		// reply
		// return ?
	}
	else if (!channel.isInChannel(clientIt->getId()))
	{
		// reply
		// return ?
	}
	else
	{
		const int id(clientIt->getId());
		status ? channel.addOperator(id) : channel.removeOperator(id);
	}
}

void Server::processLimit(const std::string arg, Channel& channel, const bool status)
{
	if (arg.empty())
	{
		// reply
		return ;
	}
	if (status)
	{
		const int limit(std::atoi(arg.c_str())); // TODO: remove atoi [!!!!!!!!!!!!!!!]
		if (limit < 1)
		{
			// reply
		}
		else
		{
			channel.setLimit(static_cast<size_t>(limit));
		}
	}
	else
	{
		channel.setLimit(0);
	}
}