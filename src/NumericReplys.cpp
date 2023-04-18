#include "NumericReplys.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

NumericReplys::NumericReplys()
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

NumericReplys::~NumericReplys()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/


/*
** --------------------------------- METHODS ----------------------------------
*/

// RPL_WELCOME (001)
void NumericReplys::rpl_welcome(const Client &client)
{
    std::string msg = ":localhost 001 ";
    msg += client.getclientnick() + " ";
    msg += ":Welcome to our IRC server (Made by dmarceli, ncameiri and amaria-m) ";
    msg += client.getNickmask();
    msg += "\n";
    send(client.getclientsocket(), msg.c_str(), msg.size(), 0);
}

// RPL_NOTOPIC (331)
void NumericReplys::rpl_notopic(const Client &client, const str channelName)
{
    std::string msg = ":localhost 001 ";
    msg += client.getclientnick() + " ";
    msg += channelName + " :No topic is set";
    msg += "\n";
    send(client.getclientsocket(), msg.c_str(), msg.size(), 0);
}

// RPL_TOPIC (332)
void NumericReplys::rpl_topic(const Client &client, const str channelName, const str &topic)
{
    (void) client;
    (void) channelName;
    (void) topic;
}

// RPL_NAMREPLY (353)
void NumericReplys::rpl_namreply(const Client &client, const Channel &channel)
{
    (void) client;
    (void) channel;
}

// RPL_ENDOFNAMES (366)
void NumericReplys::rpl_endofnames(const Client &client, const str &channelName)
{
    (void) client;
    (void) channelName;
}

// ERR_CHANOPRIVSNEEDED (482)
void NumericReplys::rpl_chanoprivsneeded(const Client &client, const str &channelName)
{
    (void) client;
    (void) channelName;
}

// Cant enter channel messages --------------------------------------------------
// ERR_BADCHANNELKEY (475), ERR_BANNEDFROMCHAN (474), ERR_CHANNELISFULL (471), ERR_INVITEONLYCHAN (473)

void NumericReplys::rpl_cannotEnterChannel(const Client &client, const str &channelName)
{
    (void) client;
    (void) channelName;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */