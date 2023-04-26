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
	if (channel_to.c_str()[0] != '#' && channel_to.c_str()[0] != '&')
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
		for (std::vector<Channel>::iterator channel = _channels.begin(); channel != _channels.end(); channel++)
		{
			if (channel->getName() == channelName)
			{
				std::size_t found = s.find(':');
				if (found != s.npos)
				{
					channel->sendMessage(*it, s.substr(found, s.size() - found), "PART");
					it->sendPrivateMsg(*it, channel->getName() + " " + s.substr(found, s.size() - found), "PART");
					channel->leave(*it);
				}
				else{
					NumericReplys().rpl_wrongcmd(*it,"ERROR");
					return;
				}
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
				_channels.push_back(Channel(channelName, ""));
				_channels.back().addUser(*it);
			}
			else
			{
				if (_channels[check].getChannelModes()["+k"][0] == "0")
					_channels[check].addUser(*it);
				else
				{
					std::stringstream keyParser(s);
					int counter = 0;
					str key;
					while (keyParser >> key)
						counter++;
					if (counter != 3 || _channels[check].getChannelModes()["+k"][1] != key)
						NumericReplys().rpl_badchannelkey(*it, _channels[check].getName());
					else
						_channels[check].addUser(*it);
					
				}
			}
		}
		catch (const std::exception &e)
		{
			std::cout << "SERVER PRINT: join throwed exeption -> " << e.what() << '\n';
		}
	}
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

void showChannelModes(const Client &client, Channel &channel)
{
	str modes = "+tn";
	if (channel.getChannelModes()["+k"][0] == "1")
		modes += "k";
	if (channel.getChannelModes()["+l"][0] == "1")
		modes += "l";
	if ((int)modes.size() > 3)
	{
		if (channel.getChannelModes()["+k"][0] == "1")
			modes += " " + channel.getChannelModes()["+k"][1];
		if (channel.getChannelModes()["+l"][0] == "1")
			modes += " " + channel.getChannelModes()["+l"][1];
	}		
	NumericReplys().rpl_channelmodeis(client, channel.getName(), modes);
}

void Msg_Handle::mode_command(std::vector<Client>::iterator it, str s)
{
	std::stringstream file(s);
	str parser;
	str channelName;
	str command;
	str parameter;
	while (file >> parser)
	{
		if (parser == "MODE" || parser == "mode")
			continue;
		else if ((int)channelName.size() == 0)
			channelName = parser;
		else if ((int)command.size() == 0)
			command = parser;
		else if ((int)parameter.size() == 0)
		{
			parameter = parser;
			while (file >> parser)
				parameter += " " + parser;
			break;
		}
	}

	std::vector<Channel>::iterator channel = get_channel_by_name(channelName);
	std::size_t nickmaskServ = parameter.find_last_of("!");
	if (nickmaskServ != parameter.npos)
		parameter = parameter.substr(0, nickmaskServ);
	std::vector<Client>::iterator client = get_client_by_name(parameter);

	int sucessfullCommand = 0;
	if (channelName.find_first_not_of(WHITESPACE) == channelName.npos) // Command: MODE
		NumericReplys().rpl_needmoreparams(*it, "MODE");
	else if (channel == _channels.end()) // Command: MODE <invalid channel>
		NumericReplys().rpl_nosuchchannel(*it, channelName);
	else if (command.find_first_not_of(WHITESPACE) == command.npos) // Command: MODE <channel>
		showChannelModes(*it, *channel);
	else if (!(channel->isChannelOperator(it->getNickmask()))) // if user isn't op, it can't use MODE command
		NumericReplys().rpl_chanoprivsneeded(*it, channelName);
	else if (parameter.find_first_not_of(WHITESPACE) == parameter.npos && (command == "+b" || command == "-b")) // Command: MODE <channel> <command>
		NumericReplys().rpl_banlist(*it, channelName, channel->getBanList());
	else if (client == _clients.end() && (command != "+k" && command != "+l" && command != "-k" && command != "-l")) // Command: MODE <channel> <+b/-b/+o/-o> <invalid nick>
		NumericReplys().rpl_nosuchnick(*it, parameter);
	else if (parameter.find_first_not_of(WHITESPACE) == parameter.npos && (command == "+k" || command == "-k" || command == "+l")) // Command: MODE <channel> <+k/-k/+l>
		NumericReplys().rpl_needmoreparams(*it, "MODE " + command);
	else if (command == "-l" && channel->channelSizeLimit() > 0) // Command: MODE <channel> -l
		channel->sizeLimitFlag(it->getNickmask(), 0);
	else if (((command == "+o" || command == "-o") && parameter.find_first_not_of(WHITESPACE) == parameter.npos) || (command == "+l" && atoi(parameter.c_str()) < (int)channel->getNumberOfUsers())) // MODE <channel> <+o/-o> |OU| MODE <channel> +l (n < channelLen) 
		std::cout << "Ignore command\n";
	else if (command == "+b") // Command: MODE <channel> +b <nick>
		sucessfullCommand = channel->addClientBanned(*it, client->getNickmask());
	else if (command == "-b") // Command: MODE <channel> -b <nick>
		sucessfullCommand = channel->rmvClientBanned(*it, client->getNickmask());
	else if (command == "+o") // Command: MODE <channel> +o <nick>
		sucessfullCommand = channel->addChannelOp(*it, client->getNickmask());
	else if (command == "-o") // Command: MODE <channel> -o <nick>
		sucessfullCommand = channel->rmvChannelOp(*it, client->getNickmask());
	else if (command == "+k") // Command: MODE <channel> +k <key>
		sucessfullCommand = channel->addChannelKey(*it, parameter);
	else if (command == "-k") // Command: MODE <channel> -k <key>
		sucessfullCommand = channel->rmvChannelKey(*it, parameter);
	else if (command == "+l")
		sucessfullCommand = channel->sizeLimitFlag(it->getNickmask(), atoi(parameter.c_str()));
	
	if (sucessfullCommand)
	{
		channel->sendMessage(*it, command + " " + parameter, "MODE");
		it->sendPrivateMsg(*it, channelName + " " + command + " " + parameter, "MODE");
	}
}

void Msg_Handle::invite_command(std::vector<Client>::iterator it, str s)
{
	std::stringstream in(s);
	str command, receiver, channel;

	in >> command;
	in >> receiver;
	in >> channel;

	std::vector<Channel>::iterator Userchan = get_channel_by_name(channel);
	if (get_client_by_name(receiver) == _clients.end())
		NumericReplys().rpl_nosuchnick(*it, receiver);
	else if (Userchan != _channels.end() && Userchan->isChannelOperator(it->getNickmask()))
		it->sendPrivateMsg(*get_client_by_name(receiver), receiver + " " + channel, command);
	else
		NumericReplys().rpl_chanoprivsneeded(*it,channel);
}

void Msg_Handle::iterate_over_clients(std::vector<Client> vect, int caller_fd)
{
	std::vector<Client>::const_iterator client_it = vect.begin();
	for (; client_it != vect.end(); client_it++)
	{
		if (client_it->getclientsocket() == caller_fd)
			continue;
		str msg = ":localhost 352 " + get_client_by_fd(caller_fd)->getclientnick() + " * " + client_it->getclientuser() + " localhost *.localhost "+ client_it->getclientnick()+ " Hx :0 "+ client_it->getRealName() +"\n";
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
			std::cout<<"WORD->"<<word<<"\n";
			if (word[0] == '#' || word[0] == '&')
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
					str msg = ":localhost 352 " + get_client_by_fd(fd)->getclientnick() + " * " + get_client_by_name(word)->getclientuser() + " localhost *.localhost "+ get_client_by_name(word)->getclientnick()+ " Hx :0 "+  get_client_by_name(word)->getRealName() +"\n";
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
			reason = ":Kicked";
		for (std::vector<Channel>::iterator channel = _channels.begin(); channel != _channels.end(); channel++)
		{
			if (channel->getName() == channelName)
			{
				if(channel->isChannelOperator(it->getNickmask()))
				{
					channel->sendMessage(*it, get_client_by_name(userName)->getclientnick() + " " + reason, "KICK");
					it->sendPrivateMsg(*it, reason, "KICK " + channelName + " " + get_client_by_name(userName)->getclientnick());
					channel->leave(*get_client_by_name(userName));
				}
				else
					NumericReplys().rpl_chanoprivsneeded(*it,channelName);
			}
		}
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
	if (in == "PONG :localhost\r\n" || in == "PONG :localhost\n")
	{
		std::cout << "PONG VERIFIED\n";
		it->is_waiting_for_pong = false;
	}
}

void Msg_Handle::names_command(str in, const Client &it)
{
	std::vector<Channel>::iterator chan_it =get_channel_by_name(in);
	if(chan_it == _channels.end())
		return;
	str msg = ":localhost 353 " + it.getclientnick() + " = " + in + " :" + chan_it->get_all_user_nicks() + "\n:localhost 366 " + it.getclientnick() + " " + in + " :End of /NAMES list.\n";
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

bool Msg_Handle::nick_already_used(str new_nick, int fd){
	 std::vector<Client>::iterator it = _clients.begin();
    if(new_nick == "NICK" || new_nick == "")
		{
			NumericReplys().rpl_wrongcmd(*get_client_by_fd(fd),"ERROR");
			return true;
		}
    for (; it != _clients.end(); it++){

		if (new_nick.size() > 9)
		{
			str msg =":localhost 432 " + new_nick + " " + new_nick + " :Erroneus nickname.\n";
			send(fd,msg.c_str(), msg.size(), 0);
			return true;
		}
        if (it->getclientnick() == new_nick && it->getclientsocket() != fd)
        {
			//:luna.AfterNET.Org 433 * rdrake :Nickname is already in use.
			str msg =":localhost 433 ";
			if(it->getclientnick() == "")
				msg += "*";
			else
				msg += it->getclientnick();
			msg += " " + new_nick + " :Nickname is already in use.\n";
			send(fd,msg.c_str(), msg.size(), 0);
            return true;
        }
    }
	return false;
}

void Msg_Handle::nick_command(str in,std::vector<Client>::iterator it)
{
    str old_nickmask = it->getNickmask();
	str msg = ":" + old_nickmask + " NICK :" + in + "\n"; 
    it->setnick(in);
    for (std::vector<Channel>::iterator channel = _channels.begin(); channel != _channels.end(); channel++){
        if(channel->userIsMemberOfChannel(old_nickmask))
        {
            channel->sendMessage(msg);
            channel->update_client_nick(it->getclientsocket(),in);
            channel->update_user_list(it->getclientsocket());
        }
	}
	send(it->getclientsocket(),msg.c_str(), msg.size(), 0);
}

void Msg_Handle::notice_command(std::vector<Client>::iterator cli_it, str msg, str receiver)
{
	size_t found = msg.find(":");
	if (get_client_by_name(receiver) == _clients.end())
		NumericReplys().rpl_nosuchnick(*cli_it, receiver);
	else if (found != std::string::npos)
 		cli_it->sendPrivateMsg(*get_client_by_name(receiver), msg.substr(found, msg.size() - found), "NOTICE " + get_client_by_name(receiver)->getclientnick());
}
