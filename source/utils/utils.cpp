/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbarros <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 18:11:15 by fbarros           #+#    #+#             */
/*   Updated: 2023/08/14 18:11:36 by fbarros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <common.hpp>
#include <ctime>

namespace Utils
{
    std::string toUpper(const std::string& original)
    {
        std::string result = original;
        for (size_t i = 0; i < original.length(); i++)
        {
            result[i] = std::toupper(original[i]);
        }
        return result;
    }

    bool isDigit(const std::string& s)
    {
        bool ret = !s.empty();

        for (std::string::const_iterator it = s.begin(); it < s.end(); it++)
        {
            ret &= (*it <= '9' && *it >= '0');
        }
        return ret;
    }

    bool writeTo(const std::string& s, const int fd)
    {
        if (send(fd, s.c_str(), s.size(), 0) < static_cast<ssize_t>(s.size()))
        {
            std::cerr << "Error: failed to send message:\n";
            return false;
        }
        return true;
    }

    std::string timeToStr(void)
    {
        time_t currentTime;
        std::string str;

        std::time(&currentTime);
        return numToStr(currentTime);
    }

    bool isLineComplete(const std::string& line)
    {
        return line.find("\n") != std::string::npos;
    }
}
