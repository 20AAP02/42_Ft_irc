#ifndef BOT_HPP
# define BOT_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <cstdlib>
# include <poll.h>
# include <cstring>
#include <algorithm>
#include <string>


# define TOKEN "XXX"
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
        void ChatGPT(str msg,str sendernick,str &response);
        str remove_from_str(str expr,str string_);
        ~Bot(){};
};


/*			COLOURS		*/

# define BLANK "\033[0m"
# define RED "\033[38;5;1m"
# define PURPLE "\033[38;5;13m"
# define YELLOW "\033[33m"
# define BLUE "\033[38;5;14m"
# define GREEN "\033[38;5;47m"
# define ORANGE "\033[38;5;208m"

#endif