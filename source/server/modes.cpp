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

void Server::replyMode(const Client& client, const std::string& channel, const std::string& param1, const std::string& param2)
{
    const std::string nick(client.getNickname());

    // "Official" reply
    if (param1 == "+l")
        broadcast(":" + client.toString() + " MODE #" + channel + " " + param1 + " :" + param2 + EOL, channel);
    else
        broadcast(":" + client.toString() + " MODE #" + channel + " :" + param1 + EOL, channel);
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
    const int id(client.getId());
    std::istringstream iss(line);
    std::string channelName;
    std::queue<std::string> modes;
    std::string args;

    iss >> channelName;
    if (channelName.empty())
    {
        addMessage("MODE :<target> [[(+|-)]<modes> [<mode-parameters>]]\r\n", id);
        return ;
    }
    if (!isChannelValid(channelName))
    {
        addMessage(channelName + " :No such channel\r\n", id);
        return ;
    }

    // insert all arguments in string
    while (!iss.eof())
    {
        iss >> args;
        if (!args.empty())
            modes.push(args);
    }

    channelName.erase(0, 1);
    Channel& channel = *(getChannel(channelName));
    if (modes.empty())
	{
    	replyChannelModeIs(client, channel);
		replyCreationTime(client, channel);
	}
    else if (!channel.isOperator(id))
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
    if (name.empty())
        return false;
    bool invalid(name.size() < 2);
    invalid |= (name.at(0) != '#');
    invalid |= (!channelExists(name.substr(1)));

    return !invalid;
}

void Server::replyModeUnknown(const Client& client, const std::string& param)
{
    const int id(client.getId());
    const std::string nick(client.getNickname());
    addMessage(":" + HOST + " " + UNKNOWNMODE + " " + nick + " " + param + " " + " :is an unknown mode character\r\n" ,id);
}

void Server::parseModes(std::queue<std::string>& modes, Channel& channel, const Client& client)
{
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
            replyMissingParam(client, channelName, token.substr(1));
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
        replyMissingParam(client, channelName, "o"), client.getId();
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
            {
                replyMode(client, channelName, "+o", user);
            }
        }
        else
        {
            if (channel.removeOperator(id))
            {
                replyMode(client, channelName, "-o", user);
            }
        }
            
	}
}

void Server::processLimit(const Client& client, const std::string arg, Channel& channel, const bool status)
{
    const std::string chanop(client.getNickname());
	
    if (arg.empty())
	{
        replyMissingParam(client, channel.getName(), "l"), client.getId();
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
