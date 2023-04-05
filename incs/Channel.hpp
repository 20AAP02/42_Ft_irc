#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <string>
# include <vector>
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

		const std::vector<Client> &getUsers() const;
        const str &getName() const;
        const str &getTopic() const;

	private:
		str _channelName;
		str _channelTopic;
        std::vector<Client> _users;
};

std::ostream &			operator<<( std::ostream & o, Channel const & i );

#endif /* ********************************************************* CHANNEL_H */