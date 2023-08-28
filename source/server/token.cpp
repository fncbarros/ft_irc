#include "../includes/Server.hpp"
#include <stdio.h>
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
        validateToken(s1);

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

void Server::validateToken(std::string& token) const
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