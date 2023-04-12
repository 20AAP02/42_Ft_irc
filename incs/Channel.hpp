#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <Client.hpp>
# include <sys/socket.h> // send()

typedef std::string	str;

class Channel
{

	public:

		Channel(const str &name, const str &topic);
		Channel( Channel const & src );
		~Channel();
		
		void addUser(const Client &user);

		// Commands for channel moderators
		void changeChannelMode(const str &mode);
		void inviteUser(const Client &user);
        void removeUser(const Client &user);
		void changeTopic(const str &newTopic);

		// Commands for all channel members
		void sendMessage(const Client &user, const str &message, const str &msgType) const;
		void modeCommand(const Client &user) const;
		void whoCommand(const Client &user) const;
		void leave(const Client &user, const str &goodbyMessage);

		// Operators
		Channel &		operator=( Channel const & rhs );

		// Accessors
		const std::vector<Client> &getUsers() const;
        const str &getName() const;
        const str &getTopic() const;
		const str &getType() const;
		const std::vector<str> &getFounders() const;
		const std::vector<str> &getHalfops() const;
		const std::vector<str> &getProtectedUsers() const;
		const std::map<str, std::vector<str> > &getChannelModes() const;

	private:
		str _channelName;
		str _channelTopic;
		str _channelType;
        std::vector<Client> _users;

		// Channel Operators
		std::vector<str> _founders;
		std::vector<str> _halfops;
		std::vector<str> _protectedUsers;

		// Channel Modes Map
		std::map<str, std::vector<str> > _channelModes;

		//Additional fucntions
		void removeFromVector(const Client &user, std::vector<str> &vector);
};

std::ostream &			operator<<( std::ostream & o, Channel const & i );

class channelErrorException : public std::exception {
	private:
		const std::string _errorMessage;
	public: 
		channelErrorException(const std::string errorMessage): _errorMessage(errorMessage) {};
		virtual const char *what() const throw() {
			return _errorMessage.c_str();
		}
		virtual ~channelErrorException() throw() {};
};

#endif /* ********************************************************* CHANNEL_H */