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
}
