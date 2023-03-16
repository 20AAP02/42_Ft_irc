#include "../incs/Server.hpp"

int main(int ac, char **av)
{
	(void)av;
	if (ac != 3)
	{
		std::cout << RED"Wrong Arguments!"BLANK << std::endl;
		std::cout << GREEN"\t->Usage :\"./ircserv <port> <password>\""BLANK << std::endl;
		return 1;
	}
}