#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <fcntl.h>
#include <errno.h>
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

typedef std::string	str;

class Server {
public:
    Server(const str& port, const str& password);
private:
    int port_;
    str password_;
    int server_socket_;
    std::vector<int> client_sockets_;
    void getPortAndPassword(const str& port, const str& password);
    void createServerSocket();
    void bindServerSocket();
    void listenForIncomingConnections();
    void handleClientCommunication();
};

/*			COLOURS		*/

# define BLANK "\033[0m"
# define RED "\033[38;5;1m"
# define PURPLE "\033[38;5;13m"
# define YELLOW "\033[33m"
# define BLUE "\033[38;5;14m"
# define GREEN "\033[38;5;47m"
# define ORANGE "\033[38;5;208m"

#endif
