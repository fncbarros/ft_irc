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

#include "includes/common.hpp"

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
}
