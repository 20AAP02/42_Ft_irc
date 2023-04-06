#include "Server.hpp"


Server::Server(const str& port, const str& password)
{
    getPortAndPassword(port, password);
    createServerSocket();
    bindServerSocket();
    listenForIncomingConnections();
    setServerPoll();
    handleClientCommunication();
}

void Server::setServerPoll()
{
    num_clients_ = 0;
    client_fds_[0].fd = this->server_socket_;
    client_fds_[0].events = POLLIN;
    fcntl(this->server_socket_, F_SETFL, O_NONBLOCK);
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
    {
        std::ostringstream oss;
        oss << "Failed to bind server socket to port " << port_;
        ft_error(oss.str());
    }
}

void Server::listenForIncomingConnections() 
{
    if (listen(server_socket_, MAX_CLIENTS) < 0) 
        ft_error("Failed to listen for incoming connections");
}

void Server::addNewClientToPoll()
{  
    int client_socket = client_sockets_.back();
    client_sockets_.pop_back();
    client_fds_[num_clients_ + 1].fd = client_socket;
    client_fds_[num_clients_ + 1].events = POLLIN;
    fcntl(client_socket, F_SETFL, O_NONBLOCK);
    num_clients_++;
}

void Server::handleNewConnection()
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_socket = accept(this->server_socket_, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_socket < 0)
        std::cerr << "Failed to accept incoming connection" << std::endl;
    client_sockets_.push_back(client_socket);
    str welcome_msg = "Welcome to Dani's and Toni's server!\n";
    send(client_socket, welcome_msg.c_str(), welcome_msg.size(), 0);
    if (num_clients_ < MAX_CLIENTS)
        addNewClientToPoll();
    else
    {
        std::cerr << "Maximum number of clients reached" << std::endl;
        close(client_socket);
    }
}


void Server::handleClientDisconnection(int i)
{
    close(client_fds_[i].fd);
    client_fds_[i].fd = -1;
    client_fds_[i].events = 0;
    num_clients_--;
    std::cout << "Client disconnected" << std::endl;
}

void Server::handleClientInput(str input)
{
    // TODO: Implement commands handling 
    std::cout << "Received command: " << input;
}

void Server::handleClientCommunication() 
{
    std::cout << GREEN "Server Started" BLANK << std::endl;
    while (1)
    {
        while (num_clients_ < MAX_CLIENTS && client_sockets_.size() > 0)
            addNewClientToPoll();
        if (poll(client_fds_, num_clients_ + 1, -1) < 0)
            ft_error("Error in poll()");     
        for (int i = 0; i <= num_clients_; i++)
        {
            if (client_fds_[i].revents == POLLIN)
            {
                if (i == 0)
                    handleNewConnection();
                else
                {
                    char buffer[BUFFER_SIZE];
                    std::memset(buffer, 0, sizeof(buffer));
                    int num_bytes = recv(client_fds_[i].fd, buffer, sizeof(buffer), 0);
                    if (!num_bytes)
                        handleClientDisconnection(i);
                    else
                        handleClientInput(buffer);
                }
            }
        }
    }
}
