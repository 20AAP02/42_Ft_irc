#include <Server.hpp>

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << RED "Wrong Arguments!" BLANK << std::endl;
		std::cout << GREEN "\t->Usage :\"./ircserv <port> <password>\"" BLANK << std::endl;
		return 1;
	}
    Server server(argv[1], argv[2]);
}