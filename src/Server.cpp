#include "Server.hpp"

Server::Server(const str& port, const str& password)
{
    getPortAndPassword(port, password);
    createServerSocket();
    bindServerSocket();
    listenForIncomingConnections();
    handleClientCommunication();
}

void ft_error(str msg)
{
    std::cerr << msg << std::endl;
    exit(0);
}

void Server::getPortAndPassword(const str& port, const str& password)
{
    for (std::string::const_iterator it = port.begin(); it != port.end(); ++it)
    {
        if (!std::isdigit(*it))
            ft_error("Wrong Input! => " + port);
    }
    this->port_ = std::atoi(port.c_str());
    this->password_ = password;
}

void Server::createServerSocket() 
{
    server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_ < 0)
        ft_error("Failed to create server socket");
}

void Server::bindServerSocket() 
{
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_);
    if (bind(server_socket_, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
        ft_error("Failed to bind server socket to port " + port_);
}

void Server::listenForIncomingConnections() 
{
    if (listen(server_socket_, MAX_CLIENTS) < 0) 
        ft_error("Failed to listen for incoming connections");
}

void Server::handleClientCommunication() 
{
    while (1)
    {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_socket = accept(this->server_socket_, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0)
        {
            std::cerr << "Failed to accept incoming connection" << std::endl;
            continue;
        }
        client_sockets_.push_back(client_socket);
        std::string welcome_msg = "Welcome to Dani's and Toni's server!\n";
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