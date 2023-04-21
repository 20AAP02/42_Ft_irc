#ifndef BOT_HPP
# define BOT_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <cstdlib>
# include <poll.h>
# include <cstring>

typedef std::string	str;

const str SERVER_ADDRESS = "localhost";
const str SERVER_PORT = "8080";
const str CHANNEL = "#example";
const str PASSWORD = "ola123";


class Bot
{
    private:
        str _ServAddress;
        int _ServPort;
        str _ServPassword;
        str _ChannelToJoin;
        int _BotSocket;
    public:
        Bot();
        //createBotSocket();
        ~Bot();
};




#endif