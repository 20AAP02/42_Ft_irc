#include <Channel.hpp>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Channel::Channel(const str &name, const str &topic)
{
	str notPermittedChars = " \f\n\r\t\v,";
	notPermittedChars.push_back(7); // 7 ascii -> ctrl G
	if (name.find_first_of(notPermittedChars) != name.npos)
		throw Channel::channelErrorException();
	if (name[0] != '#' && name[0] != '&')
		throw Channel::channelErrorException();
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
	for (std::vector<str>::const_iterator founder = src.getFounders().begin(); founder != src.getFounders().end(); founder++)
		this->_founders.push_back(*founder);
	for (std::vector<str>::const_iterator halfop = src.getHalfops().begin(); halfop != src.getHalfops().end(); halfop++)
		this->_halfops.push_back(*halfop);
	for (std::vector<str>::const_iterator protectedUser = src.getProtectedUsers().begin(); protectedUser != src.getProtectedUsers().end(); protectedUser++)
		this->_protectedUsers.push_back(*protectedUser);
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
		this->_founders.clear();
		for (std::vector<str>::const_iterator founder = rhs.getFounders().begin(); founder != rhs.getFounders().end(); founder++)
			this->_founders.push_back(*founder);
		this->_halfops.clear();
		for (std::vector<str>::const_iterator halfop = rhs.getHalfops().begin(); halfop != rhs.getHalfops().end(); halfop++)
			this->_halfops.push_back(*halfop);
		this->_protectedUsers.clear();
		for (std::vector<str>::const_iterator protectedUser = rhs.getProtectedUsers().begin(); protectedUser != rhs.getProtectedUsers().end(); protectedUser++)
			this->_protectedUsers.push_back(*protectedUser);
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
	std::string message = ":" + user.getclientnick() + "!~"; 
	message.append(user.getNickmask() + " JOIN " + this->_channelName + "\n");
	send(user.getclientsocket(), message.c_str(), message.size(), 0);
	this->_users.push_back(user);
	if ((int)this->_founders.size() == 0)
		this->_founders.push_back(user.getNickmask());
}

// -- Commands for channel moderators --


void Channel::changeChannelMode(const str& mode)
{
	(void)mode;
}

void Channel::inviteUser(const Client& user)
{
	(void)user;
}

void Channel::removeUser(const Client& user)
{
	(void)user;
}

void Channel::changeTopic(const str& newTopic)
{
	(void)newTopic;
}

// -- Commands for all channel members --

void Channel::sendMessage(const Client &user, const str &message) const
{
	(void) user;
	(void) message;
}

void Channel::modeCommand(const Client &user) const
{
	(void) user;
}

void Channel::whoCommand(const Client &user) const
{
	(void) user;
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

const std::vector<str> &Channel::getFounders() const
{
	return this->_founders;
}

const std::vector<str> &Channel::getHalfops() const
{
	return this->_halfops;
}

const std::vector<str> &Channel::getProtectedUsers() const
{
	return this->_protectedUsers;
}

const std::map<str, std::vector<str> > &Channel::getChannelModes() const
{
	return this->_channelModes;
}


/* ************************************************************************** */