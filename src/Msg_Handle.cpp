#include <Msg_Handle.hpp>


Msg_Handle::Msg_Handle()
{
    std::string public_channel = "#public";
    std::string public_topic = "Public Topic";
    _channels.push_back(Channel(public_channel, public_topic));
    num_clients = 0;
    for (int i = 1; i < MAX_CLIENTS + 1; i++)
        this->set_pollfd_clients_fd(-1, i);
};

void Msg_Handle::handlerealname(str in, std::vector<Client>::iterator it)
{
    size_t poscol = in.find(':') + 1;
    if (poscol == str::npos)
        return;
    size_t posnl = in.find("\n", poscol);
    str word = in.substr(poscol, posnl - poscol - 1);
    it->setrealname(word);
}

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
        if (word == "PASS")
        {
            s >> word;
            if (pwd_handle(word, fd, it))
            {
                return 1;
            }
        }
        else if (word == "NICK")
        {
            s >> word;
            nick_name_set(it, word);
        }
        else if (word == "USER")
        {
            s >> word;
            it->setuser(word);
            it->set_user_bool();
            handlerealname(in, it);
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
    //std::cout << "Bools de Validação nick_" << it->get_nick_bool() << " get user" << it->get_user_bool() << " Get pass" << it->get_pass_bool() << "\n";
    if (it->get_nick_bool() && it->get_user_bool() && it->get_pass_bool())
    {
        it->set_logged();
        _channels[0].addUser(*it);
        std::cout << "SERVER PRINT: " << it->getclientnick() << " ->LOGGED IN \n";
        it->set_admin(true);
        it->ping_client();
        NumericReplys().rpl_welcome(*it);
    }
    return 0;
}


int Msg_Handle::handleClientCommand(str in, int fd)
{
    std::vector<Client>::iterator it = get_client_by_fd(fd);
    std::cout << RED "[MESSAGE]: " BLANK  << YELLOW <<  in <<  BLANK;
    std::cout << "  ->Sent by " << it->getNickmask() << "[" << it->getclientsocket() << "]" << std::endl;
    std::stringstream s(in);
    str command;
    str word;
    if (!it->is_logged_in())
        return 0;
    while (s >> word)
    {
        command = word;
        s >> word;
        if (it->is_waiting_for_pong)
            handle_pong(in, it);
        else if (command == "JOIN")
            join_command(word, it, s.str());
        else if (command == "PRIVMSG")
            privmsg_handle(it, s.str(), word);
        else if (command == "NICK")
            nick_name_set(it, word);
        else if (command == "PART")
            part_command(word, it, s.str());
        else if (command == "TOPIC")
            topic_command(word, it, s.str());
        else if (command == "WHO")
            who_command(s.str(), fd);
        else if (command == "LIST")
            list_command(fd);
        else if (command == "NAMES")
            names_command(word, *it);
        else if(command == "WHOIS")
            whois_command(word, it);
		else if (command == "MODE")
            mode_command(word, it, s.str());
        else if (command == "QUIT")
            return 1;
        else if (command == "INVITE")
            invite_command(it, in);
        else if (command == "KICK")
            kick_command(it, in, fd);
    }
    return 0;
}

int Msg_Handle::check_input(str in, int fd)
{
    if (Client_login(in, fd) || handleClientCommand(in, fd))
        return 1;
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
    std::vector<Client>::iterator it = _clients.begin();
    for (; it != _clients.end(); it++)
    {
        if (it->getclientsocket() == fd)
        {
            _clients.erase(it);
            break;
        }
    }
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

void Msg_Handle::delete_client_from_channels(int fd)
{

    std::vector<Channel>::iterator it = _channels.begin();
    for (; it != _channels.end(); it++)
    {
        it->delete_users_by_fd(fd);
        it->update_user_list(fd);
    }
}


std::vector<Client>::iterator Msg_Handle::get_client_by_name(const str &name)
{
    std::vector<Client>::iterator it = _clients.begin();
    for (; it != _clients.end(); it++)
    {
        if (it->getclientnick() == name)
            return it;
    }
    return _clients.end();
}

std::vector<Channel>::iterator Msg_Handle::get_channel_by_name(const str &name)
{
    std::vector<Channel>::iterator it = _channels.begin();
    for (; it != _channels.end(); ++it)
    {
        if (it->getName() == name)
            return it;
    }
    return _channels.end();
}

bool Msg_Handle::checkPingTimeout(int fd)
{
    long current_time = time(NULL);
    std::vector<Client>::iterator it = get_client_by_fd(fd);
    if (it != _clients.end() && it->is_waiting_for_pong && current_time - it->get_time_ping() > TIMEOUT)
    {
        std::cout << "Closing connection with " << it->getNickmask() << " due to PING timeout" << std::endl;
        // delete_client_from_channels(it->getclientsocket());
        it->is_waiting_for_pong = false;
        return true;
    }
    return false;
}

void Msg_Handle::remove_pollfd_client(int fd)
{
    for (int i = 0; i <= MAX_CLIENTS; i++)
    {
        if (client_pollfd[i].fd == fd)
        {
            client_pollfd[i].fd = -1;
            client_pollfd[i].events = 0;
            client_pollfd[i].revents = 0;
            break;
        }
    }
}

Msg_Handle::~Msg_Handle(){};

/*DEBUG Funtions*/
void Msg_Handle::print_all_client_vector_or_index(int opt)
{
    std::cout << RED << "[DEBUG](print_all_client_vector_or_index)" << BLANK << std::endl;
    std::vector<Client>::iterator it = _clients.begin();
    if (opt != -1)
    {
        if (opt >= (int)_clients.size())
        {
            std::cout << RED << "[DEBUG](print_all_client_vector_or_index)" << BLANK << "out of bound\n";
            return;
        }
        else
        {
            std::advance(it, opt);
            std::cout << RED << "[DEBUG](print_all_client_vector_or_index)" << BLANK << "Index " << opt << " NIck: " << it->getclientnick() << " USER: " << it->getclientuser() << " Realname : " << it->getRealName() << " FD : " << it->getclientsocket() << "\n";
            return;
        }
    }
    opt = 0;
    for (; it != _clients.end(); it++)
    {

        std::cout << RED << "[DEBUG](print_all_client_vector_or_index)" << BLANK << "Index " << opt << " NIck: " << it->getclientnick() << " USER: " << it->getclientuser() << " Realname : " << it->getRealName() << " FD : " << it->getclientsocket() << "\n";
        opt++;
    }
}
