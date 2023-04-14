#include <Msg_Handle.hpp>

void Msg_Handle::nick_name_set(std::vector<Client>::iterator cli_it, str nick)
{
    cli_it->setnick(nick);
    cli_it->set_nick_bool();
};

void Msg_Handle::Privatemsg_handle(std::vector<Client>::iterator cli_it, str msg,str channel_to)
{
	size_t found = msg.find(":");
	if (found != std::string::npos)
		_channels[0].sendMessageToUser(*cli_it, *get_client_by_name(channel_to), msg.substr(found, msg.size() - found), "PRIVMSG");
	
}

void Msg_Handle::privmsg_handle(std::vector<Client>::iterator cli_it, str msg, str channel_to)
{

	if (channel_to.c_str()[0] != '#')
		Privatemsg_handle(cli_it,msg, channel_to);
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
        std::cout << "SERVER PRINT(pwd_handle): " << "Password correcta" << std::endl;
        it-> set_pass_bool();
    }
    else
    {
        std::cout << "SERVER PRINT(pwd_handle): " << "Password Incorrecta" << std::endl;
        std::string exit_msg = ":127.0.0.1 464 user :WrongPass\n";
        send(fd, exit_msg.c_str(), exit_msg.size(), 0);
        std::cout << "SERVER PRINT(pwd_handle): " << "Server RES: " << exit_msg;
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
		catch(const std::exception& e)
		{
			std::cout << "SERVER PRINT(excepcao join_command): " << e.what() << '\n';
		}
	}
	(void) s;
}

void Msg_Handle::mode_command(str word, std::vector<Client>::iterator it, str s)
{
	(void) word;
	(void) it;
	(void) s;
}

void Msg_Handle::invite_command(std::vector<Client>::iterator it, str s)
{
	std::size_t name_pos = s.find("INVITE ") + 7;
    std::size_t channel_pos = s.find("#");
    if (name_pos == str::npos || channel_pos == str::npos || !it->is_admin())
    	return;
    str receiver = s.substr(name_pos, channel_pos - name_pos - 1);
    str channel = s.substr(channel_pos, s.length() - channel_pos);
	channel.erase(channel.size() -1);
	_channels[0].sendMessageToUser(*it, *get_client_by_name(receiver), channel, "INVITE");
}

void Msg_Handle::kick_command(std::vector<Client>::iterator it, str s)
{
	//
	str reason, channelName, userName;
    size_t pos1 = s.find(' ');
    size_t pos2 = s.find(' ', pos1 + 1);
    size_t pos3 = s.find(':');
	if (pos3 == str::npos)
		reason = "no reason";
	else
  		reason = s.substr(pos3 + 1, reason.length()-1);
    channelName = s.substr(pos1 + 2, pos2 - pos1 - 1);
    userName = s.substr(pos2 + 1, pos3 - pos2 - 2);

	for (std::vector<Channel>::iterator channel = _channels.begin(); channel != _channels.end(); channel++)
	{
		if (channel->getName() == channelName){
			channel->leave(*it, "kicked");
			channel->sendMessageToUser(*it, *get_client_by_name(userName), reason, "KICK #" + channelName);
			for(std::vector<Client>::iterator send_to_cli = _clients.begin(); send_to_cli != _clients.end(); send_to_cli++){
				channel->sendMessage(*it, "KICK #"+get_client_by_name(userName)->getclientuser(), reason);
			}	
		}
	}
}
