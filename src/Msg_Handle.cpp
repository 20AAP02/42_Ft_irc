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
        std::cout<<"COMAND "<< command<<" Word "<<word<<"\n";
        if (word == "PASS"){
            s >> word;
            if(pwd_handle(word,fd, it)){
                return 1;
            }
        }
        else if (word == "NICK"){
             s >> word;
          nick_name_set(it,word);
        }
        else if (word == "USER")
        {
             s >> word;
            it->setuser(word);
            it->set_user_bool();
        }
        else if (word == "CAP")
        {
             s >> word;
            if (word == "LS")
            {   
                 
                std::string msg = "CAP * LS :multi-prefix sasl\n";
                send(fd, msg.c_str(), msg.size(), 0);
            }
            else if (word == "REQ")
            {
                 s >> word;
                std::string msg = "CAP * ACK multi-prefix\n";
                send(fd, msg.c_str(), msg.size(), 0);
            }
        }
    }
    std::cout<< "Bools de Validação nick_"<<it->get_nick_bool()<<" get user"<<it->get_user_bool()<<" Get pass"<<it->get_pass_bool()<<"\n";
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


void Msg_Handle::handleOperatorCommand(str in, int fd)
{
    std::vector<Client>::iterator it = get_client_by_fd(fd);
    if (!it->is_admin())
        return;
    str command, word;
    std::stringstream s(in);
    while (s >> word)
    {
        command = word;
        s >> word;
        if (command == "INVITE")
            invite_command(it, in);
        else if (command == "KICK")
            kick_command(it,in,fd);
        // else if(command == "OPER")
        //     ;
    }

}

void Msg_Handle::handleClientCommand(str in, int fd)
{
    std::vector<Client>::iterator it = get_client_by_fd(fd);
    std::cout << in;
    std::cout << "SERVER PRINT: " << "sent by " << it->getNickmask() << "["<< it->getclientsocket()<< "]"<< std::endl;
    std::stringstream s(in);
    str command;
    str word;
    if (!it->is_logged_in()){
        std::cout<<"NAO estou logado amigo(handleClientCommand)\n";
        return;
    }
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
        else if (command == "LIST")
			list_command(s.str(), fd);    
		else if(command == "QUIT")
			std::cout << "SERVER PRINT: " << "ainda nao temos o comando QUIT\n";
        /*else if(command == "WHO")
            who_command();*/
    }
}

int Msg_Handle::check_input(str in, int fd)
{
    if (Client_login(in, fd))
        return 1;
    handleClientCommand(in, fd);
    handleOperatorCommand(in,fd);
    
    /*
    421     ERR_UNKNOWNCOMMAND
    "<command> :Unknown command"
    */

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
    return this->num_clients;
}

std::vector<Channel> Msg_Handle::get_channels()
{
    return this->_channels;
}

void Msg_Handle::delete_client(int fd)
{
    if (get_client_by_fd(fd) != _clients.end())
        _clients.erase(get_client_by_fd(fd));
}

std::vector<Client>::iterator Msg_Handle::get_client_by_fd(int fd)
{
    std::vector<Client>::iterator it = _clients.begin();
    for (; it != _clients.end(); ++it)
    {
        if (it->getclientsocket() == fd)
            return it;
    }
    return _clients.end();
}

 void Msg_Handle::delete_client_to_disconnect(int fd){
   std::vector<Channel>::iterator it = _channels.begin();
    for (; it != _channels.end(); ++it)
    {
        it->removeUser(*get_client_by_fd(fd));
    }
 }

std::vector<Client>::iterator Msg_Handle::get_client_by_name(const str& name)
{
    std::vector<Client>::iterator it = _clients.begin();
    for (; it != _clients.end(); ++it)
    {
        if (it->getclientnick() == name)
            return it;
    }
    return _clients.end();
}

std::vector<Channel>::iterator Msg_Handle::get_channel_by_name(const str& name)
{
    std::vector<Channel>::iterator it = _channels.begin();
    for (; it != _channels.end(); ++it)
    {
        if (it->getName() == name)
            return it;
    }
    return _channels.end();
}

Msg_Handle::~Msg_Handle(){};