#include <string>
#include <cstdlib>

// TODO: dummy compilable source file
int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		return 1;
	}

	int port = std::atoi(argv[1]);
	
	std::string password(argv[2]);

	// temporary solution to keep it compilable
	(void)port;
	(void)password;

	return 0;
}
