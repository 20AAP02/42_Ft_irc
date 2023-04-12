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
        else if (command== "CAP")
        {
            
            if (word == "LS")
            {
                std::string msg = "CAP * LS\n";
                send(fd, msg.c_str(), msg.size(), 0);
            }
            else if (word == "REQ")
            {
                std::string msg = "CAP * ACK\n";
                send(fd, msg.c_str(), msg.size(), 0);
            }
        }
    }
    if (it->get_nick_bool() && it->get_user_bool() && it->get_pass_bool())
    {
        it->set_logged();
        _channels[0].addUser(*it);
        std::cout << it->getclientnick() << " ->LOGGED IN \n";
        it->set_admin(true);
        str welcome_msg = "Welcome to our server!\n";
        send(fd, welcome_msg.c_str(), welcome_msg.size(), 0);
    }
    return 0;
}

void Msg_Handle::handleClientCommand(str in, int fd)
{
    std::vector<Client>::iterator it = get_client_by_fd(fd);
    std::cout << "Client MSG [" << fd << "]" << in;
    std::cout << "## sent by: NICK->" << it->getclientnick() << "USER->" << it->getclientuser() << "##" << std::endl;
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
		{
			try
			{
			
				int check = 0;
				for (std::vector<Channel>::iterator channel = _channels.begin(); channel != _channels.end(); channel++)
				{
					if (channel->getName() == word)
						break;
					check++;
				}
				if (check == (int)_channels.size())
				{
					_channels.push_back(Channel(word, "no topic"));
					_channels.back().addUser(*it);
				}
				else
					_channels[check].addUser(*it);
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n';
			}
		}
		else if (word == "PRIVMSG")
		{
			s >> word;
			for (std::vector<Channel>::iterator channel = _channels.begin(); channel != _channels.end(); channel++)
			{
				if (channel->getName() == word)
				{
                    str msg = s.str();
                    std::size_t found = msg.find(':') + 1;
					channel->sendMessage(*it, msg.substr(found, msg.size() - found), "PRIVMSG");
				}
			}
		}
        else if (word == "NICK")
        {
            int check = 0;
            for (std::vector<Channel>::iterator channel = _channels.begin(); channel != _channels.end(); channel++)
            {
                if (channel->getName() == word)
                    break;
                check++;
            }
            if (check == (int)_channels.size())
            {
                _channels.push_back(Channel(word, "no topic"));
                _channels.back().addUser(*it);
            }
            else
                _channels[check].addUser(*it);
        }
        else if (command == "PRIVMSG")
            privmsg_handle(it, s.str(), word);

        else if (command == "NICK")
            nick_name_set(it, word);

		else if (word == "PART")
		{
			s >> word;
			for (std::vector<Channel>::iterator channel = _channels.begin(); channel != _channels.end(); channel++)
			{
				if (channel->getName() == word)
				{
                    str msg = s.str();
                    std::size_t found = msg.find(':') + 1;
					channel->leave(*it, msg.substr(found, msg.size() - found));
				}
			}
		}

    }
};

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

void Msg_Handle::nick_name_set(std::vector<Client>::iterator cli_it, str nick)
{
    cli_it->setnick(nick);
    cli_it->set_nick_bool();
};
void Msg_Handle::privmsg_handle(std::vector<Client>::iterator cli_it, str msg, str channel_to)
{
    for (std::vector<Channel>::iterator channel = _channels.begin(); channel != _channels.end(); channel++)
    {
        if (channel->getName() == channel_to)
        {

            std::size_t found = msg.find(':') + 1;
            channel->sendMessage(*cli_it, msg.substr(found, msg.size() - found), "PRIVMSG");
        }
    }
}

int Msg_Handle::pwd_handle(str word, int fd, std::vector<Client>::iterator it)
{
    if (word == _password || word == ":" + _password)
    {
        std::cout << "Password correcta" << std::endl;
        it-> set_pass_bool();
    }
    else
    {
        std::cout << "Password INcorrecta" << std::endl;
        std::string exit_msg = ":127.0.0.1 464 user :WrongPass\n";
        send(fd, exit_msg.c_str(), exit_msg.size(), 0);
        std::cout << "Server RES: " << exit_msg;
        return 1;
    }
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