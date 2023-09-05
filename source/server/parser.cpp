/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falmeida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 14:47:11 by falmeida          #+#    #+#             */
/*   Updated: 2023/08/12 14:47:13 by falmeida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

tokenList Server::parse(std::string buffer)
{
    std::istringstream iss(buffer);
    std::string line;
    std::vector<std::string> strList;
    tokenList list;   

    while (std::getline(iss, line)) {
        strList.push_back(line);
    }

    for (std::vector<std::string>::iterator it = strList.begin(); it != strList.end(); it++)
    {
        line = *it;
        size_t spacePosition = line.find(' ');

        if (spacePosition == std::string::npos)
           continue ;

        std::string s1(line.substr(0, spacePosition));
        std::string s2(line.substr(spacePosition + 1, (line.find('\r') - (spacePosition + 1))));

        list.push_back(tokenPair(s1, s2));
    }

    return list;
}

std::string     Server::getToken(const std::string token, tokenList processedMsg)
{
    for (tokenList::iterator it = processedMsg.begin(); it != processedMsg.end(); it++)
    {
        if (it->first == token)
            return it->second;
    }
    return "";
}