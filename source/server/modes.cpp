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


static std::string replyMissingParam(const std::string& client, const std::string& channel, const std::string& param)
{
    const std::string REPLYCODE("696");
    return ":" + HOST + " " + REPLYCODE + " " + client + " #" + channel + " " + param + " * :You must specify a parameter for the op mode. Syntax: <nick>.\r\n";
}

void Server::replyChannelModeIs(const Client& client, const Channel& channel)
{
	const int id(client.getId());
	const std::string nick(client.getNickname());
    std::stringstream ss;
	const int limit(channel.limit());
	std::string arg;
	ss << limit;
    ss >> arg;
	addMessage(":" + HOST + " " + CHANNELMODEIS + " " + nick + "#" + channel.getName() +  " :" + channel.returnModes() + ((limit > 0) ? (" :" +  arg) : "") + EOL, id);
	// need to send limit if there is one
}

void Server::replyMode(const Client& client, const std::string& channel, const std::string& param1, const std::string& param2)
{
    const int id(client.getId());
    const std::string nick(client.getNickname());
    // :ana!user@LibraIRC-rqb.ri9.75sut7.IP MODE #testingIRCforProjectPurposes :-i (same for +)
    // ana sets mode -i on #testingIRCforProjectPurposes (same for +)
    if (param2.empty())
        addMessage(":" + client.toString() + " MODE #" + channel + " :" + param1 + EOL, id);

    if (param1[0] == '+')
    {
        if (param1[1] == 'o')
        {
            addMessage(nick + " gives channel operator status to " + param2 + EOL, id);
        }
        else if (param1[1] == 'l')
        {
            addMessage(":" + client.toString() + " MODE #" + channel + " " + param1 + " :" + param2 + EOL, id);
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

std::string getNotChanopMsg(const std::string& token)
{
    if (token == "+i")
    {
        return "You must be channel operator to set channel mode i (invite only)";
    }
    else if (token == "-i")
    {
        return "You must be channel operator to unset channel mode i (invite only)";
    }
    else if (token == "+t")
    {
        return "You must be channel operator to set channel mode t (topic restriction)";
    }
    else if (token == "-t")
    {
        return "You must be channel operator to unset channel mode t (topic restriction)";
    }
    else if (token == "+k")
    {
        return "You must be channel operator to set channel mode k (key)";
    }
    else if (token == "-k")
    {
        return "You must be channel operator to unset channel mode k (key)";
    }
    else if (token == "+o")
    {
        return "You must be channel operator to set channel mode o (channel operator)";
    }
    else if (token == "-o")
    {
        return "You must be channel operator to unset channel mode o (channel operator)";
    }
    else if (token == "+l")
    {
        return "You must be channel operator to set channel mode l (limit)";
    }
    else if (token == "-l")
    {
        return "You must be channel operator to unset channel mode l (limit)";
    }
    return "";
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
        if (!args.empty())
            modes.push(args);
    }

    if (!channel.isInChannel(client.getId()))
    {
        // reply
    }
    else if (modes.empty())
	{
    // >> @time=2023-09-22T17:05:47.613Z :apocalypse.IRC4Fun.net 324 ana #testingIRCBehaviour :+nt
    // >> @time=2023-09-22T17:05:47.613Z :apocalypse.IRC4Fun.net 329 ana #testingIRCBehaviour :1695402334
	// Channel #testingIRCBehaviour modes: +nt
    // Channel #testingIRCBehaviour created on Fri Sep 22 18:05:34 2023
    	replyChannelModeIs(client, channel);
		replyCreationTime(client, channel);
	}
    else if (!channel.isOperator(client.getId()))
    {
        const std::string token(modes.front());
        replyChanopNeeded(client, channelName, getNotChanopMsg(token));
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
    else if (name[0] != '#')
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

void Server::parseModes(std::queue<std::string>& modes, Channel& channel, const Client& client)
{
    const int id(client.getId());
    const std::string channelName(channel.getName());
    const std::string validTokens("+-kotli");
    char op(0);

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

        try {
            // NOTE: token can also not include + or -, in which case a + will be assumed
            status = token.at(0) != '-';
            op = token.at(1);
        } catch (const std::exception& e) {
            (void)e;
            // reply or ignore
            return ;
        }

        if (op == 'i')
        {
            if (channel.setInviteOnly(status))
                replyMode(client, channelName, token, "");
        }
        else if (op == 't')
        {
            if (channel.setTopicRestriction(status))
                replyMode(client, channelName, token, "");

        }
        else if ((token != "+l") &&  modes.empty()) // if no argument to mode when required
        {
            addMessage(replyMissingParam(client.getNickname(), channelName, token.substr(1)), id);
            addMessage("#" + channelName + " " + token.substr(1) + " * :You must specify a parameter for the op mode. Syntax: <nick>.\r\n", id);
        }
        else if (op == 'k')
        {
            const std::string newKey(modes.front());
            modes.pop();
            if (status)
            {
                if (channel.setKey(newKey))
                {
                    replyMode(client, channelName, token, newKey);
                    const std::string msg(client.getNickname() + " sets channel keyword to " + newKey + EOL);
                    broadcast(msg, channel);
                }
            }
            else
            {
                if (channel.setNoKey())
                {
                    replyMode(client, channelName, token, newKey);
                    const std::string msg(client.getNickname() + " removes channel keyword" + EOL);
                }
            }
        }
        else if (op == 'o')
        {
            const std::string user(modes.front());
            modes.pop();
			processOperator(client, channel, user, status);
        }
        else if (op == 'l') // see syntax further
        {
			const std::string limit(modes.front());
			modes.pop();
			processLimit(client, limit, channel, status);
        }
        else // ignore
        {
            return ;
        }
    }
}

void Server::processOperator(const Client& client, Channel& channel, const std::string& user, const bool status)
{
	ConnectionsList::const_iterator clientIt = getClient(user);
    const std::string channelName(channel.getName());
    if (user.empty())
    {
        addMessage(replyMissingParam(client.getNickname(), channelName, "o"), client.getId());
        addMessage("#" + channelName + " o * :You must specify a parameter for the op mode. Syntax: <nick>.\r\n", client.getId());
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
            if (channel.addOperator(id))
                replyMode(client, channelName, "+o", user);
        }
        else
        {
            if (channel.removeOperator(id))
                replyMode(client, channelName, "-o", user);
        }
            
	}
}

void Server::processLimit(const Client& client, const std::string arg, Channel& channel, const bool status)
{
    const std::string chanop(client.getNickname());
	
    if (arg.empty())
	{
        addMessage(replyMissingParam(chanop, channel.getName(), "l"), client.getId());
        addMessage("#" + channel.getName() + " l * :You must specify a parameter for the limit mode. Syntax: <nick>.\r\n", client.getId());
		return ;
	}
	if (status)
	{
        const int limit = Utils::riskyConversion<std::string, int>(arg);
		if (limit < 1)
		{
			return ;
		}
		else
		{
			
            if (channel.setLimit(static_cast<size_t>(limit)))
            {
                replyMode(client, channel.getName(), "+l", arg);
                broadcast("#" + chanop + " sets channel limit to " + arg + EOL, channel, client.getId());
            }
		}
	}
	else
	{
        if (channel.setLimit(0))
        {
            replyMode(client, channel.getName(), "-l", arg);
            broadcast(chanop + " removes user limit" + EOL, channel, client.getId());
        }
	}
}
