#include <string>
#include <vector>
#include <iostream>

typedef std::string	str;

class Channel
{
    public:
        Channel(const str& name);
        void addUser(const str& username);
        void removeUser(const str& username);
        const std::vector<str>& getUsers() const ;
        const str& getName() const ;
    private:
        str _ChannelName;
        std::vector<str> _users;
};