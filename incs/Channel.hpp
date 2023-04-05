#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <vector>
# include <iostream>
# include <Client.hpp>

typedef std::string	str;

class Channel
{
    public:
        Channel(const str& name);
        void addUser(const Client& user);

		// Commands for channel moderators
		void changeChannelMode(const str& mode);
		void inviteUser(const Client& user);
        void removeUser(const Client& user);
		void changeTopic(const str& newTopic);


        const std::vector<Client>& getUsers() const;
        const str& getName() const;
        const str& getTopic() const;

    private:
        str _channelName;
		str _channelTopic;
        std::vector<Client> _users;
};

#endif
