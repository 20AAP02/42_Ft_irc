#include <Channel.hpp>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Channel::Channel(const str &name, const str &topic)
{
	str notPermittedChars = " \f\n\r\t\v,";
	notPermittedChars.push_back(7); // 7 ascii -> ctrl G
	if (name.find_first_of(notPermittedChars) != name.npos)
		throw channelErrorException("channel name contains invalid characters");
	if (name[0] != '#' && name[0] != '&')
		throw channelErrorException("channel name doesn't contain type character (ex: #name or &name)");
	this->_channelName = name;
	this->_channelTopic = topic;
	this->_channelType.push_back(name[0]);
	this->_channelModes.insert(std::pair<str, std::vector<str> >("+b", std::vector<str>(1, "0")));
	this->_channelModes.insert(std::pair<str, std::vector<str> >("+e", std::vector<str>(1, "0")));
	this->_channelModes.insert(std::pair<str, std::vector<str> >("+l", std::vector<str>(1, "0")));
	this->_channelModes.insert(std::pair<str, std::vector<str> >("+i", std::vector<str>(1, "0")));
	this->_channelModes.insert(std::pair<str, std::vector<str> >("+I", std::vector<str>(1, "0")));
	this->_channelModes.insert(std::pair<str, std::vector<str> >("+k", std::vector<str>(1, "0")));
	this->_channelModes.insert(std::pair<str, std::vector<str> >("+m", std::vector<str>(1, "0")));
	this->_channelModes.insert(std::pair<str, std::vector<str> >("+s", std::vector<str>(1, "0")));
	this->_channelModes.insert(std::pair<str, std::vector<str> >("+t", std::vector<str>(1, "1")));
	this->_channelModes.insert(std::pair<str, std::vector<str> >("+n", std::vector<str>(1, "1")));
}

Channel::Channel( const Channel & src ): _channelName(src.getName()), _channelTopic(src.getTopic()), _channelType(src.getType())
{
	for (std::vector<Client>::const_iterator client = src.getUsers().begin(); client != src.getUsers().end(); client++)
		this->_users.push_back(*client);
	for (std::vector<str>::const_iterator op = src.getChannelOperators().begin(); op != src.getChannelOperators().end(); op++)
		this->_channelOperators.push_back(*op);
	for (std::map<str, std::vector<str> >::const_iterator mode = src.getChannelModes().begin(); mode != src.getChannelModes().end(); mode++)
	{
		for (int i = 0; i < (int)mode->second.size(); i++)
			this->_channelModes[mode->first].push_back(mode->second[i]);
	}
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Channel::~Channel()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Channel &				Channel::operator=( Channel const & rhs )
{
	if ( this != &rhs )
	{
		this->_channelName = rhs.getName();
		this->_channelTopic = rhs.getTopic();
		this->_channelType = rhs.getType();
		this->_channelOperators.clear();
		for (std::vector<str>::const_iterator op = rhs.getChannelOperators().begin(); op != rhs.getChannelOperators().end(); op++)
			this->_channelOperators.push_back(*op);
		this->_users.clear();
		for (std::vector<Client>::const_iterator client = rhs.getUsers().begin(); client != rhs.getUsers().end(); client++)
			this->_users.push_back(*client);
		for (std::map<str, std::vector<str> >::const_iterator mode = rhs.getChannelModes().begin(); mode != rhs.getChannelModes().end(); mode++)
		{
			this->_channelModes[mode->first].clear();
			for (int i = 0; i < (int)mode->second.size(); i++)
				this->_channelModes[mode->first].push_back(mode->second[i]);
		}
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Channel const & i )
{
	o << "Channel: " << i.getName() << " (" << i.getTopic() << ")";
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

void Channel::addUser(const Client& user)
{
	if (this->userIsMemberOfChannel(user) && !this->isBanned(user.getNickmask()))
		return;
	if (this->channelSizeLimit() != 0 && this->channelSizeLimit() > (int)this->getNumberOfUsers())
		return;
	std::string message = ":" + user.getclientnick() + "!";
	message.append(user.getNickmask() + " JOIN " + this->_channelName + "\n");
	this->_users.push_back(user);
	if ((int)this->_channelOperators.size() == 0)
		this->_channelOperators.push_back(user.getNickmask());
	sendMessage(user, "", "JOIN");
	send(user.getclientsocket(), message.c_str(), message.size(), 0);
}

void Channel::sendMessage(const Client &user, const str &message, const str &msgType)
{
	if (!(this->userIsMemberOfChannel(user)))
		return;
	str msg = ":" + user.getclientnick() + "!";
	if (this->isChannelOperator(user.getNickmask()))
		msg += "@";
	msg += user.getNickmask() + " " + msgType + " " + this->_channelName + " " + message + "\n";
	for (std::vector<Client>::const_iterator member = this->_users.begin(); member != this->_users.end(); member++)
		if (member->getNickmask() != user.getNickmask())
			send(member->getclientsocket(), msg.c_str(), msg.size(), 0);
}

void Channel::sendMessageToUser(const Client& user, const Client& receiver, const str& message, const str& msgType) const 
{
    str msg = ":" + user.getclientnick() + "!~" + user.getNickmask() + " " + msgType + " " + this->_channelName + " " +  message + "\n";
    send(receiver.getclientsocket(), msg.c_str(), msg.size(), 0);
}

void Channel::topicCommand(const Client &user, const str command)
{
	std::cout << "SERVER PRINT: " << "in channel topic: command -> " << command << std::endl;
	if (!(this->userIsMemberOfChannel(user)))
		return;
	str msg = ":" + user.getclientnick() + "!~" + user.getNickmask() + " TOPIC " + this->getName();
	std::cout << "SERVER PRINT: " << "in channel topic: msg -> " << msg << std::endl;
	// Send user current topic
	if (command.find_first_not_of(" \t\n\f\r\v") == command.npos)
	{
		std::cout << "SERVER PRINT: " << "show topic" << std::endl;
		msg += " ";
		// No topic
		if (this->getTopic().find_first_not_of(" \t\n\f\r\v") == this->getTopic().npos)
			msg += "No topic is set (" + this->getTopic() + ")\n";
		else
			msg += this->getTopic() + "\n";
		send(user.getclientsocket(), msg.c_str(), msg.size(), 0);
	}
	else if (command.find_first_not_of(" \t\n\f\r\v:") == command.npos)
	{
		std::cout << "SERVER PRINT: " << "clean topic" << std::endl;
		this->changeTopic(user, "No topic is set");
	}
	else
	{
		std::cout << "SERVER PRINT: " << "change topic" << std::endl;
		str topic = command.substr(command.find(":") + 1);
		this->changeTopic(user, topic);
	}
}

void Channel::leave(const Client &user, const str &goodbyMessage)
{
	this->sendMessage(user, goodbyMessage, "PART");
	this->removeUser(user);
}


// ----------- Checker Member Functions ----------

std::size_t Channel::getNumberOfUsers() const
{
	return this->_users.size();
}

bool Channel::isChannelOperator(const str &nickMask)
{
	for (std::vector<str>::const_iterator user = this->_channelOperators.begin(); user != this->_channelOperators.end(); user++)
		if (nickMask == *user)
			return true;
	return false;
}

bool Channel::isBanned(const str &nickMask)
{
	if (this->_channelModes["+b"][0] == "0")
		return false;
	for (std::vector<str>::const_iterator mem = this->_channelModes["+b"].begin(); mem != this->_channelModes["+b"].end(); mem++)
	{
		if (*mem == nickMask)
		{
			if (this->_channelModes["+e"][0] == "0")
				return true;
			for (std::vector<str>::const_iterator exept = this->_channelModes["+e"].begin(); exept != this->_channelModes["+e"].end(); exept++)
				if (*exept == nickMask)
					return false;
			return true;
		}
	}
	return false;
}

int Channel::channelSizeLimit()
{
	if (this->_channelModes["+l"][0] == "0")
		return 0;
	return atoi(this->_channelModes["+l"][1].c_str());
}

bool Channel::wasInvited(const str &nickMask)
{
	for (std::vector<str>::const_iterator mem = this->_channelModes["+i"].begin(); mem != this->_channelModes["+i"].end(); mem++)
		if (*mem == nickMask)
			return true;
	return false;
}

const str Channel::getKey()
{
	if (this->_channelModes["+k"][0] == "0")
		return "";
	return this->_channelModes["+k"][1];
}

bool Channel::canSpeak(const str &nickMask)
{
	if (this->_channelModes["+n"][0] == "0")
		return true;
	for (std::vector<Client>::iterator member = this->_users.begin(); member != this->_users.end(); member++)
		if (member->getNickmask() == nickMask)
			return true;
	return false;
}

bool Channel::isChannelSecret()
{
	if (this->_channelModes["+s"][0] == "1")
		return true;
	return false;
}

bool Channel::canChangeTopic(const str &nickMask)
{
	if (this->_channelModes["+t"][0] == "0")
		return true;
	return this->isChannelOperator(nickMask);
}


// ----------- Private Member Funtions -----------

void Channel::removeFromVector(const Client &user, std::vector<str> &vector)
{
	for (std::vector<str>::iterator member = vector.begin(); member != vector.end(); member++)
	{
		if (*member == user.getNickmask())
		{
			vector.erase(member);
			break;
		}
	}
}

int Channel::userIsMemberOfChannel(const Client &user) const
{
	for (std::vector<Client>::const_iterator member = this->_users.begin(); member != this->_users.end(); member++)
		if (member->getNickmask() == user.getNickmask())
			return 1;
	return 0;	
}

void Channel::removeUser(const Client& user)
{
	for (std::vector<Client>::iterator member = this->_users.begin(); member != this->_users.end(); member++)
	{
		if (user.getNickmask() == member->getNickmask())
		{
			this->_users.erase(member);
			this->removeFromVector(user, this->_channelOperators);
			break;
		}
	}
}

void Channel::changeTopic(const Client &user, const str& newTopic)
{
	if (!(this->canChangeTopic(user.getNickmask())))
		return;
	this->_channelTopic = newTopic;
	this->sendMessage(user, this->getTopic(), "TOPIC");
	this->sendMessageToUser(user, user, this->getTopic(), "TOPIC");
}

void Channel::changeChannelMode(const str& mode)
{
	(void)mode;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

const std::vector<Client> &Channel::getUsers() const
{
	return this->_users;
}

const str &Channel::getName() const
{
	return this->_channelName;
}

const str &Channel::getTopic() const
{
	return this->_channelTopic;
}

const str &Channel::getType() const
{
	return this->_channelType;
}

const std::vector<str> &Channel::getChannelOperators() const
{
	return this->_channelOperators;
}

const std::map<str, std::vector<str> > &Channel::getChannelModes() const
{
	return this->_channelModes;
}


/* ************************************************************************** */