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



class Bot
{
    private:
        str _ServPort;
        str _ServPassword;
        str _ChannelToJoin;
        int _BotSocket;
        bool _is_ponged ;
    public:
        Bot(str ServPort, str ServPassword, str Channeltojoin);
        void createBotSocket();
        void connectToServer();
        void RegisterInServer();
        void MainConnectionLoop();
        void HandleServerInput(char *buffer, int num_bytes);
        void HandlePRIVMSG(str buf);
        str ChatGPT(str buf);
        //~Bot();
};




#endif