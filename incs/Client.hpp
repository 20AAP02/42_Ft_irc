#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>

typedef std::string str;

class Client
{
private:
    str _message;
    str _nick;
    str _user;
    str _realname;
    long _time_ping;
    bool _is_admin;
    int _socket_fd;
    bool _is_logged_in;
    bool _password;
    bool _nick_cl;
    bool _user_cl;
    // struct pollfd client_pollfd;
    // Channels with access list std::list<Channel>

public:
    bool is_waiting_for_pong;
    Client(str nick, int socket_fd);
    Client(str nick, str user, int socket_fd);
    ~Client();

    /*void set_pollfd_fd(int in);
    void set_pollfd_revents(int in);
    void set_pollfd_events(int in);

    struct pollfd get_client_poll();*/
    void  ping_client();

    const str getclientnick() const;
    const str getclientuser() const;
    const str &getmessage() const;

    void setnick(str new_nick);
    void setuser(str new_user);
    void setrealname(str realname);
    bool is_admin() const;
    bool is_logged_in() const;
    int getclientsocket() const;
    void set_message(str message);
    void set_admin(bool switch_admin);
    void set_logged();
    void set_pass_bool();
    void set_nick_bool();
    void set_user_bool();
    bool get_pass_bool();
    bool get_nick_bool();
    bool get_user_bool();
    long get_time_ping();
    str getRealName();
    str getNickmask() const;

    // Add Channel to list
    // Remove Channel to list
};
#endif