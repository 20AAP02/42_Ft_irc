#include <string>
#include <vector>
#include <iostream>

typedef std::string	str;

class Channel
{
    public:
        Channel(const str& name);
        void addUser(const str& userName);

		// Commands for channel moderators
		void changeChannelMode(const str& mode);
		void inviteUser(const str& userName);
        void removeUser(const str& userNme);
		void changeTopic(const str& newTopic);


        const std::vector<str>& getUsers() const;
        const str& getName() const;

    private:
        str _channelName;
		str _channelTopic;
        std::vector<str> _users;
};