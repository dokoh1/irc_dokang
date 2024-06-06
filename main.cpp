#include "IRCServer.hpp"

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cerr << "IRC will have 3 argument" << std::endl;
		return 1;
	}

	IRCServer server(argv[1], argv[2]);
	server.run();

	return 0;
}