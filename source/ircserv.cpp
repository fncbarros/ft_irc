/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbarros <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 19:21:18 by fbarros           #+#    #+#             */
/*   Updated: 2023/08/10 19:21:20 by fbarros          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <iostream>
#include <csignal>
#include "includes/Server.hpp"

Server server;

void signal_handler(int signal)
{
	if (signal == SIGINT)
		server.interrupt();
}

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cout << "Invalid argments: ./ircserv [port] [password]" << std::endl;
		return (1);
	}
	std::signal(SIGINT, signal_handler);
	
	if (server.setConnection(std::atoi(argv[1]), argv[2]) > 0)
	{
		std::cout << "Failed to setup the server connection" << std::endl;
		return (1);
	}
	server.connectionLoop();
}
