#include <string>
#include <cstdlib>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <iostream>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// TODO: dummy compilable source file
int main(int argc, char* argv[])
{
	int sckt;
	struct sockaddr_in sckt_addr;
	socklen_t sckt_len = sizeof(sckt_addr);
	if (argc < 3)
	{
		return 1;
	}

	int port = std::atoi(argv[1]);
	
	std::string password(argv[2]);

	if ((sckt = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		std::cout << "Failed to create the socket" << std::endl;
		return (1);
	}

	sckt_addr.sin_family = AF_INET;
	sckt_addr.sin_port = htons(port);
	sckt_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
	
	if (bind(sckt, (sockaddr *)&sckt_addr, sckt_len) < 0)
	{
		std::cout << "Failed to bind the socket" << std::endl;
		return (1);
	}

	if (listen(sckt, 20) < 0)
	{
		std::cout << "Failed to listen the socket" << std::endl;
		return (1);
	}

	int new_sckt = accept(sckt, (sockaddr *)&sckt_addr, &sckt_len);
	
	if (new_sckt < 0)
	{
		std::cout << "Failed to accept the socket" << std::endl;
		return (1);
	}

	std::cout << "Connection Established" << std::endl; 
	close(sckt);
	close(new_sckt);
	return 0;
}
