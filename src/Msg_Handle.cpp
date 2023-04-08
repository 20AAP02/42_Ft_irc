#include <Msg_Handle.hpp>

typedef std::string str;
Msg_Handle::Msg_Handle()
{
    std::string public_channel = "#public";
    std::string public_topic = "Public Topic";
    _channels.push_back(Channel(public_channel, public_topic));
    num_clients = 0;
};

void Msg_Handle::check_input(str in, Server server_)
{
    if (in == "ola")
        std::cout << "tes";
    (void)server_;
    if (in.find("CAP LS") != std::string::npos)
    {
        std::cout << "Encontrei" << std::endl;
    }
    /*Reposta ao CAP LS302
    CAP * LS :
    */
    /*
    CAP LS 302
    PASS 123
    NICK nickname
    USER username 0 * :realname
    JOIN #canal
    PRIVMSG :conversada
    PART é o /leave
    QUIT :Leaving
    */

    /*
    MODE #channel
    WHO #channel
    KICK
    INVITE
    TOPIC
    LIST
    */
};

void Msg_Handle::check_input(str in, int fd)
{
    std::cout << in << std::endl;
    if (in.find("PASS") != std::string::npos)
    {
        if (in.substr(5, this->_password.size()) == this->_password)
        {
            // PASSword correcta
            std::cout << "Encontrei" << std::endl;
        }
        else
        {
            std::string exit_msg = ":127.0.0.1 464 nunouser :WrongPass\n";
            send(fd, exit_msg.c_str(), exit_msg.size(), 0);
        }
    }

    // if(in.find(""))
}

void Msg_Handle::add_cli_num(){
    num_clients++;
};
void Msg_Handle::del_cli_num(){
    num_clients--;
};

int Msg_Handle::get_cli_num(){
    return num_clients;
};

struct pollfd *Msg_Handle::get_client_poll()
{
    return this->client_pollfd;
}

void Msg_Handle::set_pollfd_server_fd(int fd)
{

    client_pollfd[0].fd = fd;
}
void Msg_Handle::set_pollfd_server_events(int events)
{

    client_pollfd[0].events = events;
}
void Msg_Handle::set_pollfd_server_revents(int revents)
{

    client_pollfd[0].revents = revents;
}

void Msg_Handle::set_pollfd_clients_revents(int revents, int index)
{

    client_pollfd[index].revents = revents;
}

void Msg_Handle::set_pollfd_clients_events(int events, int index)
{

    client_pollfd[index].events = events;
}

void Msg_Handle::set_pollfd_clients_fd(int fd, int index)
{

    client_pollfd[index].fd = fd;
}

int Msg_Handle::get_client_socket_last()
{
    return _clients.back().getclientsocket();
}

int Msg_Handle::get_pollfd_clients_revents(int index)
{

    return client_pollfd[index].revents;
}

int Msg_Handle::get_pollfd_clients_events(int index)
{

    return client_pollfd[index].events;
}

int Msg_Handle::get_pollfd_clients_fd(int index)
{

    return client_pollfd[index].fd;
}

void Msg_Handle::delete_last_client()
{

    _clients.pop_back();
}

void Msg_Handle::add_client(int fd)
{
    _clients.push_back(Client("", fd));
}

void Msg_Handle::set_password(str pass)
{
    this->_password = pass;
};

str Msg_Handle::get_password()
{
    return this->_password;
}
int Msg_Handle::num_of_clients()
{
    return this->_clients.size();
}

void Msg_Handle::delete_client(int fd)
{
    std::list<Client>::iterator it = _clients.begin();
    for (; it != _clients.end(); ++it)
    {
        if (it->getclientsocket() == fd)
        {
            _clients.erase(it);
            return;
        }
    }
}

Msg_Handle::~Msg_Handle(){};