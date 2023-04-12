#include "Server.hpp"
#include "Msg_Handle.hpp"

Msg_Handle msg_handler;
int run  = 1;


Server::Server(const str &port, const str &password)
{
    msg_handler.set_password(password);
    getPortAndPassword(port);
    createServerSocket();
    bindServerSocket();
    listenForIncomingConnections();
    setServerPoll();
    handleClientCommunication();
}

void Server::setServerPoll()
{
    msg_handler.set_pollfd_clients_fd(this->server_socket_, 0);
    msg_handler.set_pollfd_clients_events(POLLIN, 0);
    msg_handler.set_pollfd_clients_revents(POLLIN, 0);
    fcntl(this->server_socket_, F_SETFL, O_NONBLOCK);
}

void ft_error(str msg)
{
    std::cerr << msg << std::endl;
    exit(0);
}

void Server::getPortAndPassword(const str &port)
{
    for (std::string::const_iterator it = port.begin(); it != port.end(); ++it)
    {
        if (!std::isdigit(*it))
            ft_error("Wrong Input! => " + port);
    }
    this->port_ = std::atoi(port.c_str());
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

 
    int client_socket = msg_handler.get_client_socket_last();
    //msg_handler.delete_last_client();

    msg_handler.set_pollfd_clients_fd(client_socket, msg_handler.get_cli_num() + 1);
    msg_handler.set_pollfd_clients_events(POLLIN, msg_handler.get_cli_num() + 1);

    fcntl(client_socket, F_SETFL, O_NONBLOCK);
    msg_handler.add_cli_num();
}

void Server::handleNewConnection()
{
    
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_socket = accept(this->server_socket_, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_socket < 0)
        std::cerr << "Failed to accept incoming connection" << std::endl;
    // client_sockets_.push_back(client_socket);
    // msg_handler.add_client(client_socket);
    if (msg_handler.get_cli_num() < MAX_CLIENTS)
    {
        msg_handler.add_client(client_socket);
        addNewClientToPoll();
    }
    else
    {
        std::cerr << "Maximum number of clients reached" << std::endl;
        close(client_socket);
        msg_handler.delete_client(client_socket);
    }
}

void Server::handleClientDisconnection(int i)
{
    close(msg_handler.get_pollfd_clients_fd(i));
    msg_handler.set_pollfd_clients_fd(-1, i);
    msg_handler.set_pollfd_clients_events(0, i);
    msg_handler.delete_client(i);
    msg_handler.del_cli_num();
    std::cout << "SERVER PRINT: " << "Client disconnected" << std::endl;
}

void Server::signal_handler(int sig)
{
    (void)sig;
    run = 0;
    for (int i = 1; i <= msg_handler.get_cli_num(); i++)
        close(msg_handler.get_pollfd_clients_fd(i));
}


void Server::handleClientCommunication()
{
    signal(SIGINT, &Server::signal_handler);
    std::cout << GREEN "Server Started" BLANK << std::endl;
    while (run)
    {
        if (poll(msg_handler.client_pollfd, msg_handler.get_cli_num() + 1, -1) < 0 && run)
            ft_error("Error in poll()");
        for (int i = 0; i <= msg_handler.get_cli_num(); i++)
        {
            if (msg_handler.get_pollfd_clients_revents(i) == POLLIN)
            {
                if (i == 0)
                {
                    handleNewConnection();
                }
                else
                {
                    char buffer[BUFFER_SIZE];
                    std::memset(buffer, 0, sizeof(buffer));
                    int num_bytes = recv(msg_handler.get_pollfd_clients_fd(i), buffer, sizeof(buffer), 0);
                    if (!num_bytes)
                        handleClientDisconnection(i);
                    else
                    {
                        if(msg_handler.check_input(buffer, msg_handler.get_pollfd_clients_fd(i)))
                            handleClientDisconnection(i);
                    }
                }
            }
        }
    }
    close(server_socket_);
    std::cout << RED "Server Closed!" BLANK << std::endl;
    exit(0);
}
