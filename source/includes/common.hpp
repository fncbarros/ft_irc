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

// Includes
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string>
#include <netinet/in.h>
#include <vector>
#include <unistd.h>

// Type Definitions
typedef struct sockaddr s_sockaddr;
typedef struct sockaddr_in s_sockaddr_in;
typedef int socket_t;
const int BUFFER_SIZE = 30720;

// Const Definitions
static const char *ADDRESS = "0.0.0.0";
