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
#include <Server.hpp>
#include <common.hpp>

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
        std::cerr << "Invalid arguments\nUsage: ./ircserv [port] [password]" << std::endl;
        return 1;
    }
    const std::string port(argv[1]);
    const std::string password(argv[2]);

    if (Utils::isDigit(port) == false)
    {
        std::cerr << "Error: " << port << " is not a number\n";
        return 1;
    }

	std::signal(SIGINT, signal_handler);

	if (server.setConnection(std::atoi(argv[1]), argv[2]) > 0)
	{
		std::cerr << "Failed to setup the server connection" << std::endl;
		return 1;
	}
	server.connectionLoop();
}
