#include <Client.hpp>

typedef std::string str;

Client::Client(str nick, int socket_fd) : _nick(nick), _socket_fd(socket_fd)
{
    this->_is_admin = false;
};

Client::Client(str nick, str user, int socket_fd) : _nick(nick), _user(user), _socket_fd(socket_fd)
{
    this->_is_admin = false;
};

Client::~Client(){};

const str &Client::getclientnick() const
{
    return this->_nick;
};

const str &Client::getclientuser() const
{
    return this->_user;
};

const str &Client::getmessage() const
{
    return this->_message;
};

bool Client::is_admin() const
{
    return this->_is_admin;
};

int Client::getclientsocket() const
{
    return this->_socket_fd;
}
void Client::set_message(str message)
{
    this->_message = message;
}

void Client::set_admin(bool switch_admin)
{
    this->_is_admin = switch_admin;
}

void Client::setnick(str new_nick)
{
    this->_nick = new_nick;
}

void Client::setuser(str new_user)
{
    this->_user = new_user;
}
