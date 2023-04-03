#include <string>
#include <vector>
#include <iostream>

class Channel {
public:
    Channel(const std::string& name);
    void addUser(const std::string& username);
    void removeUser(const std::string& username);
    const std::vector<std::string>& getUsers() const ;
    const std::string& getName() const ;
private:
    std::string ChannelName_;
    std::vector<std::string> users_;
};