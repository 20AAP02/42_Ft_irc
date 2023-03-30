#include <Server.hpp>

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << RED "Wrong Arguments!" BLANK << std::endl;
		std::cout << GREEN "\t->Usage :\"./ircserv <port> <password>\"" BLANK << std::endl;
		return 1;
	}
    int port = std::atoi(argv[1]);
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        std::cerr << "Failed to create server socket" << std::endl;
        return 1;
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "Failed to bind server socket to port " << port << std::endl;
        return 1;
    }
    if (listen(server_socket, MAX_CLIENTS) < 0)
    {
        std::cerr << "Failed to listen for incoming connections" << std::endl;
        return 1;
    }
    std::vector<int> client_sockets;
    while (1)
    {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0)
        {
            std::cerr << "Failed to accept incoming connection" << std::endl;
            continue;
        }
        client_sockets.push_back(client_socket);
        std::string welcome_msg = "Welcome to Dani and Toni server!\n";
        send(client_socket, welcome_msg.c_str(), welcome_msg.size(), 0);
        char buffer[BUFFER_SIZE];
        while (1)
        {
            std::memset(buffer, 0, sizeof(buffer));
            int num_bytes = recv(client_socket, buffer, sizeof(buffer), 0);
            if (num_bytes < 0)
            {
                std::cerr << "Failed to receive data from client" << std::endl;
                break;
            }
            else if (num_bytes == 0)
            {
                std::cout << "Client disconnected" << std::endl;
                break;
            }
            std::cout << "Received command: " << buffer << std::endl;
            // TODO: Implement commands handling 
        }
    }
}