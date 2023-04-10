#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <Client.hpp>

typedef std::string	str;

class Channel
{

	public:

		Channel(const str &name, const str &topic);
		Channel( Channel const & src );
		~Channel();
		
		void addUser(const Client& user);

		// Commands for channel moderators
		void changeChannelMode(const str& mode);
		void inviteUser(const Client& user);
        void removeUser(const Client& user);
		void changeTopic(const str& newTopic);

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

		class channelErrorException : public std::exception {
			public: virtual const char* what() const throw() {
				return "Coudn't create channel.";
			}
		};

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
};

std::ostream &			operator<<( std::ostream & o, Channel const & i );

#endif /* ********************************************************* CHANNEL_H */