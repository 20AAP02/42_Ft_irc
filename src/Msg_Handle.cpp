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

void Msg_Handle::Client_login(str in, int fd)
{
    std::stringstream s(in);
    str word;
    std::vector<Client>::iterator it = get_client_by_fd(fd);

    while(s >> word)
    {
        if (word == "PASS")
        {
            s >> word;
            if (word == _password || word == ":"+_password)
            {
                it->set_logged();
                std::cout << "Password correcta" << std::endl;
            }
            else
            {
                std::cout << "Password INcorrecta" << std::endl;
                std::string exit_msg = ":127.0.0.1 464 user :WrongPass\n";
                send(fd, exit_msg.c_str(), exit_msg.size(), 0);
                std::cout << "Server RES: " << exit_msg;
                return ;
            }
        }
        else if (word == "NICK")
        {
            s >> word;
            it->setnick(word);
            it->set_nick_bool();
        }
        else if (word == "USER")
        {
            s >> word;
            it->setuser(word);
            it->set_user_bool();
        }
		else if (word == "JOIN")
		{
			s >> word;
			Channel chann(word, "no topic");
            //Adicionar o channel à lista
            _channels.push_back(chann);
			chann.addUser(*it);
		}
    }
    if (!it->is_admin() && it->get_nick_bool() && it->get_user_bool() && it->is_logged_in())
    {
        // std::string join_msg = ":" + it->getclientnick() + "!" + it->getclientuser() + "@localhost JOIN :#nuns\r\n";
        // send(fd, join_msg.c_str(), join_msg.size(), 0);
        /*std::string msg1 = ":" + it->getclientnick() + "!" + it->getclientuser() + "@localhost " + it->getclientnick() + "=#nuns:@" + it->getclientnick() + "\n:" + it->getclientnick() + "!" + it->getclientuser() + "@localhost" + it->getclientnick() + " #nuns\n:End of /NAMES list\n:" + it->getclientnick() + "!" + it->getclientuser() + "@localhost JOIN :#nuns\n: realname\n:" + it->getclientnick() + "!" + it->getclientuser() + "@localhost " + it->getclientnick() + " " + it->getclientuser() + " :End of /WHO list\r";
        send(fd, msg1.c_str(), msg1.size(), 0);*/
        _channels[0].addUser(*it);
        std::cout << it->getclientnick() <<" ->LOGGED IN \n";
        it->set_admin(true);
    }

    std::cout << "Client MSG [" <<fd<<"]" << in << std::endl;
    std::cout << "## sent by: NICK->" << it->getclientnick() << "USER->" << it->getclientuser() << "##" << std::endl ;

}

int Msg_Handle::check_input(str in, int fd)
{

    Client_login(in, fd);
    /*std::vector<Client> clients = _channels.back().getUsers();
    std::find(_channels.back().getUsers().begin(),_channels.back().getUsers().end(), Client("",fd));*/
    //!it->is admin é só para não estar a repetir isto vaias vezes,
    //terá de ser obtido atraves do get users do channel e verificar se o user já está no public
    return 0;
}

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