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

void validateToken(std::string& token)
{
    size_t i;

    std::string tmp = Utils::toUpper(token);

    for (i = 0; i < token_num; i++)
    {
        if (possible_tokens[i] == tmp)
        {
            tmp = possible_tokens[i];
            break ;
        }
    }
    if (i < token_num)
        token = tmp;
}

tokenPair Server::parse(std::string buffer)
{
    std::istringstream iss(buffer);
    std::string line;

    std::getline(iss, line);

    size_t spacePosition = line.find(' ');

    // if no space within line
    if (spacePosition == std::string::npos)
    {
        // TODO: return message (PRIVMSG or Error)
        return tokenPair("", line);
    }

    std::string s1(line.substr(0, spacePosition));
    std::string s2(line.substr(spacePosition + 1, (line.find('\r') - (spacePosition + 1))));
    validateToken(s1);

    return tokenPair(s1, s2);
}
