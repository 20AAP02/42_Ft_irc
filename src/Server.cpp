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
    std::cout << GREEN "Server Started" BLANK << std::endl;
    struct pollfd client_fds[MAX_CLIENTS + 1];
    int num_clients = 0;
    client_fds[0].fd = this->server_socket_;
    client_fds[0].events = POLLIN;

    while (1)
    {
        while (num_clients < MAX_CLIENTS && client_sockets_.size() > 0)
        {
            int client_socket = client_sockets_.back();
            client_sockets_.pop_back();

            client_fds[num_clients + 1].fd = client_socket;
            client_fds[num_clients + 1].events = POLLIN;

            num_clients++;
        }

        int ret = poll(client_fds, num_clients + 1, -1);
        if (ret < 0)
        {
            std::cerr << "Error in poll()" << std::endl;
            break;
        }
        for (int i = 0; i <= num_clients; i++)
        {
            if (client_fds[i].revents & POLLIN)
            {
                if (i == 0) // Server socket is ready 
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

                    // Add new client socket to the pollfd array
                    if (num_clients < MAX_CLIENTS)
                    {
                        client_fds[num_clients + 1].fd = client_socket;
                        client_fds[num_clients + 1].events = POLLIN;

                        num_clients++;
                    }
                    else
                    {
                        std::cerr << "Maximum number of clients reached" << std::endl;
                        close(client_socket);
                    }
                }
                else // Client socket is ready for recv()
                {
                    char buffer[BUFFER_SIZE];
                    std::memset(buffer, 0, sizeof(buffer));
                    int num_bytes = recv(client_fds[i].fd, buffer, sizeof(buffer), 0);
                    if (num_bytes < 0)
                    {
                        std::cerr << "Failed to receive data from client" << std::endl;
                        continue;
                    }
                    else if (num_bytes == 0)
                    {
                        std::cout << "Client disconnected" << std::endl;
                        close(client_fds[i].fd);

                        // Remove client socket from the pollfd array
                        client_fds[i].fd = -1;
                        client_fds[i].events = 0;
                        num_clients--;
                    }
                    else
                    {
                        std::cout << "Received command: " << buffer << std::endl;

                        // TODO: Implement commands handling 
                    }
                }
            }
        }
    }
}
