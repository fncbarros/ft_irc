#include <iostream>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdlib>

#include "Server.hpp"

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		return 1;
	}

	Server server(std::atoi(argv[1]), argv[2]);
	server.setConnection();
	server.connectionLoop();
}
