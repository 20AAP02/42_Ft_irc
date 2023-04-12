#include <Msg_Handle.hpp>

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
            std::size_t found = msg.find(':');
            channel->sendMessage(*cli_it, msg.substr(found, msg.size() - found), "PRIVMSG");
        }
    }
}

int Msg_Handle::pwd_handle(str word, int fd, std::vector<Client>::iterator it)
{
    if (word == _password || word == ":" + _password)
    {
        std::cout << "SERVER PRINT: " << "Password correcta" << std::endl;
        it-> set_pass_bool();
    }
    else
    {
        std::cout << "SERVER PRINT: " << "Password Incorrecta" << std::endl;
        std::string exit_msg = ":127.0.0.1 464 user :WrongPass\n";
        send(fd, exit_msg.c_str(), exit_msg.size(), 0);
        std::cout << "SERVER PRINT: " << "Server RES: " << exit_msg;
        return 1;
    }
    return 0;
}

void Msg_Handle::part_command(str word, std::vector<Client>::iterator it, str s)
{
    for (std::vector<Channel>::iterator channel = _channels.begin(); channel != _channels.end(); channel++)
    {
        if (channel->getName() == word)
        {
            std::size_t found = s.find(':');
            channel->leave(*it, s.substr(found, s.size() - found));
        }
    }
}

void Msg_Handle::join_command(str word, std::vector<Client>::iterator it, str s)
{
	try
	{
		(void) s;
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
		std::cout << "SERVER PRINT: " << e.what() << '\n';
	}
}