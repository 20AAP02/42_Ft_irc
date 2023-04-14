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
#include <sstream>

#include <Channel.hpp>


typedef std::string str;

class Msg_Handle
{
private:
    std::vector<Client> _clients;
    std::vector<Channel> _channels;
    str _password;
    int num_clients;

public:
    //Para já fica como public
    struct pollfd client_pollfd[MAX_CLIENTS + 1];
    struct pollfd *get_client_poll();

    Msg_Handle();
    int Client_login(str in, int fd);
    void handleClientCommand(str in, int fd);
    void handleOperatorCommand(str in, int fd);
    // 0 keep connection alive or 1 to disconnect
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
    void delete_client_to_disconnect(int fd);
    void delete_last_client(void);
    std::vector<Client>::iterator get_client_by_fd(int fd);
    std::vector<Client>::iterator get_client_by_name(const str& name);
    std::string get_password();
    ~Msg_Handle();

    /*MESSAGES
    Apenas as que desligam o client retornam ints
    */
    void nick_name_set(std::vector<Client>::iterator cli_it,str nick);
    void privmsg_handle(std::vector<Client>::iterator cli_it, str msg,str channel_to);
    void Privatemsg_handle(std::vector<Client>::iterator cli_it, str msg,str channel_to);
    int pwd_handle(str word, int fd, std::vector<Client>::iterator it);
    void part_command(str word, std::vector<Client>::iterator it, str s);
	void join_command(str word, std::vector<Client>::iterator it, str s);
	void mode_command(str word, std::vector<Client>::iterator it, str s);
    void invite_command(std::vector<Client>::iterator it, str s);
    void kick_command(std::vector<Client>::iterator it, str s);
};

#endif