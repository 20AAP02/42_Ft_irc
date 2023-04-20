#include <Msg_Handle.hpp>

void Msg_Handle::nick_name_set(std::vector<Client>::iterator cli_it, str nick)
{
	cli_it->setnick(nick);
	cli_it->set_nick_bool();
};

void Msg_Handle::Privatemsg_handle(std::vector<Client>::iterator cli_it, str msg, str channel_to)
{
	size_t found = msg.find(":");
	if (found != std::string::npos)
		cli_it->sendPrivateMsg(*get_client_by_name(channel_to), msg.substr(found, msg.size() - found), "PRIVMSG " + get_client_by_name(channel_to)->getclientnick());
}

void Msg_Handle::privmsg_handle(std::vector<Client>::iterator cli_it, str msg, str channel_to)
{

	if (channel_to.c_str()[0] != '#')
		Privatemsg_handle(cli_it, msg, channel_to);
	for (std::vector<Channel>::iterator channel = _channels.begin(); channel != _channels.end(); channel++)
	{
		if (channel->getName() == channel_to)
		{
			std::size_t found = msg.find(':');
			if (found != std::string::npos)
            	channel->sendMessage(*cli_it, msg.substr(found, msg.size() - found), "PRIVMSG");
        }
    }
}

int Msg_Handle::pwd_handle(str word, int fd, std::vector<Client>::iterator it)
{
	if (word == _password || word == ":" + _password)
	{
		std::cout << "SERVER PRINT: "
				  << "Password correcta" << std::endl;
		it->set_pass_bool();
	}
	else
	{
		std::cout << "SERVER PRINT: "
				  << "Password Incorrecta" << std::endl;
		std::cout << "PASS:" << _password << "TRY:" << word << std::endl;
		std::string exit_msg = ":127.0.0.1 464 user :WrongPass\n";
		send(fd, exit_msg.c_str(), exit_msg.size(), 0);
		std::cout << "SERVER PRINT: "
				  << "Server RES: " << exit_msg;
		return 1;
	}
	return 0;
}

void Msg_Handle::part_command(str word, std::vector<Client>::iterator it, str s)
{
	std::stringstream file(word);
	str channelName;
	while (getline(file, channelName, ','))
	{
		if (word.find(":") != word.npos)
			return;
		for (std::vector<Channel>::iterator channel = _channels.begin(); channel != _channels.end(); channel++)
		{
			if (channel->getName() == channelName)
			{
				std::size_t found = s.find(':');
				if (found != s.npos)
					channel->leave(*it, s.substr(found, s.size() - found));
			}
		}
	}
	for (std::vector<Channel>::iterator channel = _channels.begin(); channel != _channels.end(); channel++)
	{
		if (channel->getUsers().size() == 0)
		{
			_channels.erase(channel);
			break;
		}
	}
}

void Msg_Handle::join_command(str word, std::vector<Client>::iterator it, str s)
{
	std::stringstream file(word);
	str channelName;
	while (getline(file, channelName, ','))
	{
		int check = 0;
		for (std::vector<Channel>::iterator channel = _channels.begin(); channel != _channels.end(); channel++)
		{
			if (channel->getName() == channelName)
				break;
			check++;
		}
		try
		{
			if (check == (int)_channels.size())
			{
				_channels.push_back(Channel(channelName, "no topic"));
				_channels.back().addUser(*it);
			}
			else
				_channels[check].addUser(*it);
		}
		catch (const std::exception &e)
		{
			std::cout << "SERVER PRINT: join throwed exeption -> " << e.what() << '\n';
		}
	}
	(void)s;
}

void Msg_Handle::topic_command(str word, std::vector<Client>::iterator it, str s)
{
	for (std::vector<Channel>::iterator channel = _channels.begin(); channel != _channels.end(); channel++)
	{
		if (channel->getName() == word)
		{
			std::size_t found = s.find(':');
			if (found != s.npos)
				channel->topicCommand(*it, s.substr(found, s.size() - found));
			else
				channel->topicCommand(*it, "");
			break;
		}
	}
}

void Msg_Handle::mode_command(str word, std::vector<Client>::iterator it, str s)
{
	(void) it;
	std::stringstream file(s);
	str part;
	str mode;
	int sucess = 0;
	while (getline(file, part, ' '))
	{
		if (part == "MODE" || part == word)
			continue;
		mode = part;
		getline(file, part, ' ');
		str user = part.substr(0, part.find_first_of(" \n\r"));
		if (mode == "+b")
		{
			sucess = get_channel_by_name(word)->addClientBanned(*it, get_client_by_name(user)->getNickmask());
			if (sucess)
				kick_command(it, "KICK " + word + " " + user + " :Banned from this channel\n", it->getclientsocket());
		}
		else if (mode == "-b")
			sucess = get_channel_by_name(word)->rmvClientBanned(*it, get_client_by_name(user)->getNickmask());
		else if (mode == "+o")
			sucess = get_channel_by_name(word)->addChannelOp(*it, get_client_by_name(user)->getNickmask());
		else if (mode == "-o")
			sucess = get_channel_by_name(word)->rmvChannelOp(*it, get_client_by_name(user)->getNickmask());
		if (sucess)
		{
			get_channel_by_name(word)->sendMessage(*it, mode + " " + part, "MODE");
			it->sendPrivateMsg(*it, word + " " + mode + " " + part, "MODE");
		}
	}
}

void Msg_Handle::invite_command(std::vector<Client>::iterator it, str s)
{
	std::size_t name_pos = s.find("INVITE ") + 7;
	std::size_t channel_pos = s.find("#");
	if (name_pos == str::npos || channel_pos == str::npos || !it->is_admin())
		return;
	str receiver = s.substr(name_pos, channel_pos - name_pos - 1);
	str channel = s.substr(channel_pos, s.length() - channel_pos);
	channel.erase(channel.size() - 1);
	it->sendPrivateMsg(*get_client_by_name(receiver), channel, "INVITE");
}

void Msg_Handle::iterate_over_clients(std::vector<Client> vect, int caller_fd)
{
	std::vector<Client>::const_iterator client_it = vect.begin();
	for (; client_it != vect.end(); client_it++)
	{
		if (client_it->getclientsocket() == caller_fd)
			continue;
		str msg = ":localhost 352 " + get_client_by_fd(caller_fd)->getclientnick() + " * " + client_it->getclientuser() + " localhost *.localhost "+ client_it->getclientnick()+ " Hx :0 "+ "AFAZERrealname"+"\n";
		send(caller_fd, msg.c_str(), msg.size(), 0);
	}
}

void Msg_Handle::who_command(str in, int fd)
{
	std::stringstream s(in);
	str word;
	bool with_args = false;
	// WHO withouth arguments
	s >> word;
	if (s >> word)
		with_args = true;
	if (!with_args || word == "0" || word == "*")
		iterate_over_clients(_clients, fd);
	else
	{
		try
		{
			if (word[0] == '#')
			{
				str _channel = word.substr(1, word.length() - 1);
				for (std::vector<Channel>::iterator channel = _channels.begin(); channel != _channels.end(); channel++)
				{
					if (channel->getName() == word)
					{
						std::vector<Client> temp = channel->getUsers();
						iterate_over_clients(temp, fd);
					}
				}
			}

			else
			{
				if (get_client_by_name(word) != _clients.end())
				{
					std::cout << "Details About " << word << " asked by " << get_client_by_fd(fd)->getclientnick() << "\n";
					str msg = ":localhost 352 " + get_client_by_fd(fd)->getclientnick() + " * " + get_client_by_name(word)->getclientuser() + " localhost *.localhost "+ get_client_by_name(word)->getclientnick()+ " Hx :0 "+ "AFAZERrealname"+"\n";
					send(fd, msg.c_str(), msg.size(), 0);
				}
				else
				{
					std::cout << "Comando /WHO " + word + " return 0 results\n";
				}
			}
		}
		catch (std::exception &e){
			std::cout << "ERROR on WHO" << e.what() << "\n";}
	}
	str msg = ":localhost 315 " + get_client_by_fd(fd)->getclientnick()+ ' ' + get_client_by_fd(fd)->getclientnick()+ " :End of /WHO list.\n";
	send(fd, msg.c_str(), msg.size(), 0);
}

void Msg_Handle::kick_command(std::vector<Client>::iterator it, str s, int fd)
{
	str reason, channelName, userName, word;
	std::stringstream in(s);
	bool isReason = false;
	while (in >> word)
	{
		if (word == "KICK")
		{
			in >> channelName;
			in >> userName;
			if (in >> reason)
				isReason = true;
		}
	}
	try
	{
		if (isReason)
			reason = s.substr(s.find(":"), s.length() - 2);
		else
			reason = ":No reason";
		std::cout << "REASON: " << reason << "BOOL " << isReason << std::endl;
		for (std::vector<Channel>::iterator channel = _channels.begin(); channel != _channels.end(); channel++)
		{
			if (channel->getName() == channelName)
			{
				channel->leave(*get_client_by_name(userName), "kicked");
			}
		}
		it->sendPrivateMsg(*get_client_by_name(userName), reason, "KICK " + channelName + " " + get_client_by_name(userName)->getclientnick());
	}
	catch (...)
	{
		std::cout << "Erro no Formato do Kick" << std::endl;
		std::string err_msg = ":127.0.0.1 461 user :Bad Format Command\n";
		send(fd, err_msg.c_str(), err_msg.size(), 0);
	}
}
str itoa(int num)
{
    std::string str;
    bool is_negative = false;
    if (num < 0) {
        is_negative = true;
        num = -num;
    }
    while (num > 0) {
        char digit = static_cast<char>('0' + num % 10);
        str.insert(0, 1, digit);
        num /= 10;
    }
    if (is_negative)
        str.insert(0, 1, '-');
    else if (str.empty())
        str = "0";
    return str;
}

void Msg_Handle::list_command(int fd) 
{
    str msg;
    std::vector<Channel> channels = get_channels();

    for (size_t i = 0; i < channels.size(); i++)
        msg += ":localhost 322 " + get_client_by_fd(fd)->getclientnick() + " " + channels[i].getName() + " " + itoa(channels[i].getNumberOfUsers()) + " :" + channels[i].getTopic() + "\n";
    msg += ":localhost 323 " + get_client_by_fd(fd)->getclientnick() + " :End of /LIST\n";
    send(fd, msg.c_str(), msg.size(), 0);
}

void Msg_Handle::handle_pong(str in,std::vector<Client>::iterator it)
{
	if (in == "PONG :localhost\r\n")
		it->is_waiting_for_pong = false;
}

void Msg_Handle::names_command(str in, const Client &it)
{
	Channel UserChan = *get_channel_by_name(in);
	str msg = ":localhost 353 " + it.getclientnick() + " = " + in + " :" + UserChan.get_all_user_nicks() + "\n:localhost 366 " + it.getclientnick() + " " + in + " :End of /NAMES list.\n";
	send(it.getclientsocket(), msg.c_str(), msg.size(), 0);
}

void Msg_Handle::whois_command(str in,  std::vector<Client>::iterator asker)
{
	std::vector<Client>::const_iterator asked  = get_client_by_name(in);
	if (asked == _clients.end())
		return;
	str msg = ":localhost 311 " + asker->getclientnick() + " " + asked->getclientnick() + " " + asked->getclientuser() + " localhost * :" + asked->getRealName() + "\n" ;
	msg += ":localhost 312 "  + asker->getclientnick() + " " + asked->getclientnick() + " *.our_IRC_server\n" ;
	msg += ":localhost 671 " + asker->getclientnick() + " " + asked->getclientnick() + " :is connected via SSL\n"; 
	msg += ":localhost 318 " + asker->getclientnick() + " " + asked->getclientnick() + " :End of /WHOIS list.\n";
	send(asker->getclientsocket(), msg.c_str(), msg.size(), 0);
}