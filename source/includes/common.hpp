/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falmeida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 14:47:24 by falmeida          #+#    #+#             */
/*   Updated: 2023/08/12 14:47:30 by falmeida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Includes
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string>
#include <netinet/in.h>
#include <vector>
#include <unistd.h>
#include <iostream>
#include <sstream>

// Const data
static const size_t token_num = 12;
static const std::string possible_tokens[token_num] =
{
    "JOIN",
    "KICK",
    "INVITE",
    "TOPIC",
    "MODE",
    "USER",
    "PASS",
    "NICK",
    "LIST",
    "WHO",
    "QUIT",
    "PRIVMSG",
};

// Utils
namespace Utils
{
    std::string toUpper(const std::string& original);
} // namespace Utils
