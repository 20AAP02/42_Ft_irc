#include <Msg_Handle.hpp>
typedef std::string str;
Msg_Handle::Msg_Handle()
{
    std::string public_channel = "#public";
    std::string public_topic = "Public Topic";
    _channels.push_back(Channel(public_channel, public_topic));
};

void Msg_Handle::check_input(str in)
{
    if (in == "ola")
        std::cout << "tes";
    /*
    CAP LS 302
    PASS 123
    NICK nickname
    USER username 0 * :realname
    JOIN #canal
    PRIVMSG :conversada
    QUIT :Leaving
    */

    /*
    MODE #channel
    WHO #channel
    KICK
    INVITE
    TOPIC
    LIST
    */
};

void Msg_Handle::set_password(str pass)
{
    this->_password = pass;
};

str Msg_Handle::get_password()
{
    return this->_password;
}
Msg_Handle::~Msg_Handle(){};