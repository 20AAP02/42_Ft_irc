#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>

typedef std::string str;

class Client
{
private:
    str _message;
    str _nick;
    str _user;
    bool _is_admin;
    int _socket_fd;
    bool _is_logged_in;
    bool _password;
    bool _nick_cl;
    bool _user_cl;
   // struct pollfd client_pollfd;
    // Channels with access list std::list<Channel>

public:
    Client(str nick, int socket_fd);
    Client(str nick, str user, int socket_fd);
    ~Client();


    /*void set_pollfd_fd(int in);
    void set_pollfd_revents(int in);
    void set_pollfd_events(int in);

    struct pollfd get_client_poll();*/

    const str &getclientnick() const;
    const str &getclientuser() const;
    const str &getmessage() const;

    void setnick(str new_nick);
    void setuser(str new_user);
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

    // Add Channel to list
    // Remove Channel to list
};
#endif