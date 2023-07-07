#include <string>

// TODO: dummy compilable source file
int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		return 1;
	}

	int port = std::atoi(argv[1]);
	
	std::string password(argv[2]);


	return 0;
}
