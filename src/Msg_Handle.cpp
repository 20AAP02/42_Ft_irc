#include <Msg_Handle.hpp>

typedef std::string str;
Msg_Handle::Msg_Handle()
{
    std::string public_channel = "#public";
    std::string public_topic = "Public Topic";
    _channels.push_back(Channel(public_channel, public_topic));
    num_clients = 0;
};

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

int Msg_Handle::Client_login(str in, int fd)
{
    std::stringstream s(in);
    str command;
    str word;
    std::vector<Client>::iterator it = get_client_by_fd(fd);
    if (it->is_logged_in())
        return 0;
    while (s >> word)
    {
        command = word;
        s>> word;
        if (command == "PASS"){
            if(pwd_handle(word,fd, it)){
                return 1;
            }
        }
        else if (command == "NICK")
           nick_name_set(it,word);
        else if (command == "USER")
        {
            
            it->setuser(word);
            it->set_user_bool();
        }
        // else if (command == "CAP")
        // {
            
        //     if (word == "LS")
        //     {
        //         std::string msg = "CAP * LS\n";
        //         send(fd, msg.c_str(), msg.size(), 0);
        //     }
        //     else if (word == "REQ")
        //     {
        //         std::string msg = "CAP * ACK\n";
        //         send(fd, msg.c_str(), msg.size(), 0);
        //     }
        // }
    }
    if (it->get_nick_bool() && it->get_user_bool() && it->get_pass_bool())
    {
        it->set_logged();
        _channels[0].addUser(*it);
        std::cout << "SERVER PRINT: " << it->getclientnick() << " ->LOGGED IN \n";
        it->set_admin(true);
        str welcome_msg = "Welcome to our server!\n";
        send(fd, welcome_msg.c_str(), welcome_msg.size(), 0);
    }
    return 0;
}


void Msg_Handle::handleClientCommand(str in, int fd)
{
    std::vector<Client>::iterator it = get_client_by_fd(fd);
    // std::cout << "SERVER PRINT: " << "Client MSG [" << fd << "]" << in;
    std::cout << in;
    std::cout << "SERVER PRINT: " << "sent by " << it->getNickmask() << std::endl;
    std::stringstream s(in);
    str command;
    str word;
    if (!it->is_logged_in())
        return;
    while (s >> word)
    {
        command = word;
        s >> word;
        if (command == "JOIN")
			join_command(word, it, s.str());
        else if (command == "PRIVMSG")
            privmsg_handle(it, s.str(), word);
        else if (command == "NICK")
            nick_name_set(it, word);
		else if (command == "PART")
            part_command(word, it, s.str());
		else if (command == "MODE")
			mode_command(word, it, s.str());
        else if (command == "INVITE")
			std::cout << "SERVER PRINT: " << "ainda nao temos o comando INVITE\n";
		else if(command == "QUIT")
			std::cout << "SERVER PRINT: " << "ainda nao temos o comando QUIT\n";
    }
}

int Msg_Handle::check_input(str in, int fd)
{

    // Handle client disconnect
    if (Client_login(in, fd))
        return 1;
    handleClientCommand(in, fd);
    // std::vector<Client>::iterator it = get_client_by_fd(fd);
    // std::cout <<  "is logged: " << it->is_logged_in() << std::endl;
    return 0;
};


void Msg_Handle::add_cli_num()
{
    num_clients++;
};
void Msg_Handle::del_cli_num()
{
    num_clients--;
};

int Msg_Handle::get_cli_num()
{
    return num_clients;
};

struct pollfd *Msg_Handle::get_client_poll()
{
    return this->client_pollfd;
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
    if (get_client_by_fd(fd) != _clients.end())
        _clients.erase(get_client_by_fd(fd));
}

std::vector<Client>::iterator Msg_Handle::get_client_by_fd(int fd)
{
    std::vector<Client>::iterator it = _clients.begin();
    // for (; it != _clients.end(); ++it)
    // {

    //     std::cout << " começo ciclo" << it->getclientsocket() << ' ' << std::endl;
    // }
    // it = _clients.begin();

    for (; it != _clients.end(); ++it)
    {
        if (it->getclientsocket() == fd)
        {

            return it;
        }
    }

    return _clients.end();
}

Msg_Handle::~Msg_Handle(){};