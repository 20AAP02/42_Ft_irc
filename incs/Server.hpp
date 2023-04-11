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
#include <sstream>
#include <signal.h>

# include <Channel.hpp>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

typedef std::string	str;

class Server {
public:
    Server(const str& port, const str& password);
private:
    int port_;
    int server_socket_;
    struct pollfd server_pollfd;
    void getPortAndPassword(const str& port);
    void createServerSocket();
    void bindServerSocket();
    void listenForIncomingConnections();
    void setServerPoll();
    void handleClientCommunication();
    void handleNewConnection();
    void addNewClientToPoll();
    void handleClientDisconnection(int iter);
    static void signal_handler(int sig);
};

# include <cstdio> // perror()
# include <cerrno> // errno, EINTR
# include <arpa/inet.h>

/*			COLOURS		*/

# define BLANK "\033[0m"
# define RED "\033[38;5;1m"
# define PURPLE "\033[38;5;13m"
# define YELLOW "\033[33m"
# define BLUE "\033[38;5;14m"
# define GREEN "\033[38;5;47m"
# define ORANGE "\033[38;5;208m"

#endif
