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
#include <strings.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <string.h>
#include <vector>
#include <map>

#include <Channel.hpp>

#include "utils.tpp"

// Forward declarations
class Channel;

// Const data
const std::string EOL("\r\n");
static const std::string HOST("FT_IRC");
static const std::string DOMAIN("42Lisboa.com");
static const size_t token_num(13);
static const std::string possible_tokens[token_num] =
{
    "JOIN",
    "KICK",
    "INVITE",
    "TOPIC",
    "MODE",
    "USER",
    "NICK",
    "LIST",
    "WHO",
    "QUIT",
    "PRIVMSG",
    "PART",
};

// utils.cpp
namespace Utils
{
    std::string toUpper(const std::string& original);
    bool        isDigit(const std::string& s);
    bool        writeTo(const std::string& s, const int fd);
    std::string timeToStr(void);
    bool isLineComplete(const std::string& line);
} // namespace Utils
