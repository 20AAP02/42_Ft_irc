#ifndef MSG_HANDLE_HPP
#define MSG_HANDLE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <Client.hpp>
#include <Channel.hpp>
#include <Server.hpp>
#include <list>

typedef std::string str;

class Msg_Handle
{
private:
    std::list<Client> _clients;
    std::list<Channel> _channels;
    str _password;
    int num_clients;

public:
    struct pollfd client_pollfd[MAX_CLIENTS + 1];

    struct pollfd *get_client_poll();
    Msg_Handle();

    //0 keep connection alive or 1 to disconnect
    int check_input(str in, Server server_);
    int check_input(str in, int fd);
    void set_password(str pass);

    void add_client(int fd);

   

    void set_pollfd_clients_fd(int fd, int index);
    void set_pollfd_clients_events(int events, int index);
    void set_pollfd_clients_revents(int revents, int index);

    int get_pollfd_clients_fd(int index);
    int get_pollfd_clients_events(int index);
    int get_pollfd_clients_revents(int index);

    void delete_client(int fd);
    int get_client_socket_last();
    int num_of_clients();

    void del_cli_num();
    void add_cli_num();
    int get_cli_num();

    void delete_last_client(void);

    std::string get_password();
    ~Msg_Handle();
};

#endif