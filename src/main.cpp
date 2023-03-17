#include <Server.hpp>

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << RED "Wrong Arguments!" BLANK << std::endl;
		std::cout << GREEN "\t->Usage :\"./ircserv <port> <password>\"" BLANK << std::endl;
		return 1;
	}
	int _port = atoi(argv[1]);
	int obj_server, sock, reader;
	struct sockaddr_in address;
	int opted = 1;
	int address_length = sizeof(address);
	char buffer[1024] = {0};
	std::string message = argv[1] + std::string(argv[2]);
	if (( obj_server = socket (AF_INET, SOCK_STREAM, 0)) == 0)
		return EXIT_FAILURE;
	if ( setsockopt(obj_server, SOL_SOCKET, SO_REUSEADDR, &opted, sizeof(opted)))
		return EXIT_FAILURE;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(static_cast<uint16_t>(_port));
	if (bind(obj_server, (struct sockaddr *)&address, sizeof(address))<0)
		return EXIT_FAILURE;
	if (listen( obj_server, 3) < 0)
		return EXIT_FAILURE;
	if ((sock = accept(obj_server, (struct sockaddr *)&address, (socklen_t*)&address_length)) < 0)
		return EXIT_FAILURE;
	reader = read(sock, buffer, 1024);
	std::cout << buffer << std::endl;
	send(sock, message.c_str(), message.size(), 0);
	return 0;
}
