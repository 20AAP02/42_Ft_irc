#include <Channel.hpp>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Channel::Channel(const str &name, const str &topic)
{
	this->_channelName = name;
	this->_channelTopic = topic;
}

Channel::Channel( const Channel & src ): _channelName(src.getName()), _channelTopic(src.getTopic())
{
	for (std::vector<Client>::const_iterator client = src.getUsers().begin(); client != src.getUsers().end(); client++)
		this->_users.push_back(*client);
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
		this->_users.clear();
		for (std::vector<Client>::const_iterator client = rhs.getUsers().begin(); client != rhs.getUsers().end(); client++)
			this->_users.push_back(*client);
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

}

// Commands for channel moderators

void Channel::changeChannelMode(const str& mode)
{
	
}

void Channel::inviteUser(const Client& user)
{
	
}

void Channel::removeUser(const Client& user)
{
	
}

void Channel::changeTopic(const str& newTopic)
{
	
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


/* ************************************************************************** */