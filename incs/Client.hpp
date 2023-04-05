#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

typedef std::string str;

class Client
{
private:
    str _message;
    str _nick;
    str _user;
    bool _is_admin;
    int _socket_fd;
    // Channels with access list std::list<Channel>

public:
    Client(str nick, int socket_fd);
    Client(str nick, str user, int socket_fd);
    ~Client();

    const str &getclientnick() const;
    const str &getclientuser() const;
    const str &getmessage() const;

    void setnick(str new_nick);
    void setuser(str new_user);
    bool is_admin() const;
    int getclientsocket() const;
    void set_message(str message);
    void set_admin(bool switch_admin);
    // Add Channel to list
    // Remove Channel to list
};
#endif