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
#include "includes/Server.hpp"

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cout << "Invalid argments: ./ircserv [port] [password]" << std::endl;
		return 1;
	}

	Server server(std::atoi(argv[1]), argv[2]);
	server.connectionLoop();
}
