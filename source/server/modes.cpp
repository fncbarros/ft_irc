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

void Server::replyChannelModeIs(const Client& client, const Channel& channel)
{
	const int id(client.getId());
	const std::string nick(client.getNickname());
	const int limit(channel.limit());
	addMessage(":" + HOST + " " + CHANNELMODEIS + " " + nick + "#" + channel.getName() +  " :" + channel.returnModes() + ((limit > 0) ? (" :" +  std::itoa(limit)) : ""), id);
	// need to send limit is there is one
}

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
    else if (modes.empty())
	{
		replyChannelModeIs(client, channel);
		replyCreationTime(client, channel);
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

void Server::replyModeUnknown(const Client& client, const std::string& param)
{
    const int id(client.getId());
    const std::string nick(client.getNickname());
    addMessage(":" + HOST + " " + UNKNOWNMODE + " " + nick + " " + param + " " + " :is an unknown mode character\r\n" ,id);
}

void Server::replyMode(const Client& client, const std::string& channel, const std::string& param1, const std::string& param2)
{
    const int id(client.getId());
    const std::string nick(client.getNickname());
    // :ana!user@LibraIRC-rqb.ri9.75sut7.IP MODE #testingIRCforProjectPurposes :-i (same for +)
    // ana sets mode -i on #testingIRCforProjectPurposes (same for +)
    addMessage(":" + client.toString() + " MODE #" + channel + " :" + param1 + EOL, id);
    if (param1[0] == '+')
    {
        if (param1[1] == 'o')
        {
            addMessage(nick + " gives channel operator status to " + param2 + EOL, id);
        }
        else
        {
            addMessage(nick + " sets mode " + param1 + " on #" + channel + EOL, id);
        }
    }
    else
    {
        if (param1[1] == 'o')
        {
            addMessage(nick + " removes channel operator status from " + param2 + EOL, id);
        }
        else
        {
            addMessage(nick + " sets mode " + param1 + " on #" + channel + EOL, id);
        }
    }
}

void Server::parseModes(std::queue<std::string>& modes, Channel& channel, const Client& client)
{
	(void)client;
    // const int id(client.getId());
    const std::string validTokens("+-kotli");

    // i, t, k, o, l
    while (!modes.empty())
    {
        const std::string token = modes.front();
        modes.pop();
        bool status(true);

        // if bad token
        if (validTokens.find(token[0]) == std::string::npos)
        {
            replyModeUnknown(client, token);
            return ;
        }

        // NOTE: token can also not include + or -, in which case a + will be assumed
        status = token.at(0) != '-';

        if (token.at(1) == 'i')
        {
            channel.setInviteOnly(status);
            replyMode(client, channel.getName(), token, "");
        }
        else if (token.at(1) == 't')
        {
            channel.setTopicRestriction(status);
            replyMode(client, channel.getName(), token, "");

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
        else // ignore
        {
            return ;
        }
    }
}

static std::string replyMissingParam(const std::string& client, const std::string& channel)
{
    const std::string REPLYCODE("696");
    return ":" + HOST + " " + REPLYCODE + " " + client + " #" + channel + " o * :You must specify a parameter for the op mode. Syntax: <nick>.\r\n";
}

void Server::processOperator(const Client& client, Channel& channel, const std::string& user, const bool status)
{
	ConnectionsList::const_iterator clientIt = getClient(user);
    if (user.empty())
    {
        addMessage(replyMissingParam(client.getNickname(), channel.getName()), client.getId());
        addMessage("#" + channel.getName() + " o * :You must specify a parameter for the op mode. Syntax: <nick>.\r\n", client.getId());
    }
	else if (clientIt == _connections.end())
	{
        replyNoSuchNick(client, user);
	}
	else if (!channel.isInChannel(clientIt->getId()))
	{
        replyNoSuchNick(client, user);
	}
	else
	{
		const int id(clientIt->getId());
		if (status)
        {
            channel.addOperator(id);
            replyMode(client, channel.getName(), "+o", user);
        }
        else
        {
            channel.removeOperator(id);
            replyMode(client, channel.getName(), "-o", user);
        }
            
	}
}

void Server::processLimit(const std::string arg, Channel& channel, const bool status)
{
	if (arg.empty())
	{
		// reply :irc01-white.librairc.net 696 ana #testingIRCforProjectPurposes l * :You must specify a parameter for the limit mode. Syntax: <limit>
		// reply #testingIRCforProjectPurposes l * :You must specify a parameter for the limit mode. Syntax: <limit>.
		return ;
	}
	if (status)
	{
		const int limit(std::atoi(arg.c_str())); // TODO: remove atoi [!!!!!!!!!!!]
		if (limit < 1)
		{
			return ;
		}
		else
		{
			channel.setLimit(static_cast<size_t>(limit));
            // :ana!user@LibraIRC-rqb.ri9.75sut7.IP MODE #testingIRCforProjectPurposes +l :1
            //  ana sets channel limit to 1
		}
	}
	else
	{
		channel.setLimit(0);
        // :bea!user@LibraIRC-rqb.ri9.75sut7.IP MODE #testingIRCforProjectPurposes :-l
        // bea removes user limit
	}
}