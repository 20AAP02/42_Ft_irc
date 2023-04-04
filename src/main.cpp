#include <Server.hpp>
#include <Client.hpp>

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << RED "Wrong Arguments!" BLANK << std::endl;
		std::cout << GREEN "\t->Usage :\"./ircserv <port> <password>\"" BLANK << std::endl;
		return 1;
	}

	std::cout << "TESTES" << std::endl;
	Client joao("Joao", "sou o João", 2);
	std::cout << joao.getclientuser() << std::endl;
	joao.set_message("AAA");
	std::cout << joao.getmessage() << std::endl;
	std::cout << joao.is_admin() << std::endl;
	joao.set_admin(true);
	std::cout << joao.is_admin() << std::endl;
	std::cout << joao.getclientsocket() << std::endl;
	std::cout << joao.getclientnick() << std::endl;

	Server server(argv[1], argv[2]);
}