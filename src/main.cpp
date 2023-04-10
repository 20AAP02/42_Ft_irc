#include <Server.hpp>
#include <Client.hpp>
#include <Channel.hpp>

#include <list>

std::list<Channel> _gobalChannels; // global variable with all channels

template <typename T>
void printVector(const std::vector<T> &vec)
{
	std::cout << "Vector: (";
	for (typename std::vector<T>::const_iterator it = vec.begin(); it != vec.end(); it++)
	{
		std::cout << *it << ", ";
	}
	if ((int)vec.size() > 0)
		std::cout << "\b\b";
	std::cout << ")\n";
}

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
