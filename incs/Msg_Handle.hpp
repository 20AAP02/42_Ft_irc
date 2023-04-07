#ifndef MSG_HANDLE_HPP
# define MSG_HANDLE_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <Client.hpp>
# include <Channel.hpp>
#include <list>

typedef std::string	str;

class Msg_Handle
{
    private:
        std::list <Client> _clients;
        std::list <Channel> _channels;
        str _password;
    public:
        Msg_Handle();

        void check_input(str in);
        void set_password(str pass);

        std::string get_password();
        ~Msg_Handle();
};

#endif