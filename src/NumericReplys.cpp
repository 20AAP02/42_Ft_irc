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
int NumericReplys::rpl_welcome(const Client &client)
{
    std::string msg = ":localhost 001 ";
    msg += client.getclientnick() + " ";
    msg += ":Welcome to our IRC server (Made by dmarceli, ncameiri and amaria-m) ";
    msg += "\n";
    send(client.getclientsocket(), msg.c_str(), msg.size(), 0);
	return 0;
}

// RPL_NOTOPIC (331)
int NumericReplys::rpl_notopic(const Client &client, const str channelName)
{
    std::string msg = ":localhost 331 ";
    msg += client.getclientnick() + " ";
    msg += channelName + " :No topic is set";
    msg += "\n";
    send(client.getclientsocket(), msg.c_str(), msg.size(), 0);
	return 0;
}

// RPL_TOPIC (332)
int NumericReplys::rpl_topic(const Client &client, const str channelName, const str &topic)
{
    std::string msg = ":localhost 332 ";
    msg += client.getclientnick() + " ";
    msg += channelName + " :" + topic;
    msg += "\n";
    send(client.getclientsocket(), msg.c_str(), msg.size(), 0);
	return 0;
}

// RPL_NAMREPLY (353)
int NumericReplys::rpl_namreply(const Client &client, const str &channel)
{
    (void) client;
    (void) channel;
	return 0;
}

// RPL_ENDOFNAMES (366)
int NumericReplys::rpl_endofnames(const Client &client, const str &channelName)
{
    (void) client;
    (void) channelName;
	return 0;
}

// ERR_CHANOPRIVSNEEDED (482)
int NumericReplys::rpl_chanoprivsneeded(const Client &client, const str &channelName)
{
    std::string msg = ":localhost 482 ";
    msg += client.getclientnick() + " ";
    msg += channelName + " :You're not channel operator";
    msg += "\n";
    send(client.getclientsocket(), msg.c_str(), msg.size(), 0);
	return 0;
}

// ERR_BADCHANNELKEY (475)
int NumericReplys::rpl_badchannelkey(const Client &client, const str &channelName)
{
	std::string msg = ":localhost 475 ";
    msg += client.getclientnick() + " ";
    msg += channelName + " :Cannot join channel (+k)";
    msg += "\n";
    send(client.getclientsocket(), msg.c_str(), msg.size(), 0);
	return 0;
}

// ERR_BANNEDFROMCHAN (474)
int NumericReplys::rpl_bannedfromchan(const Client &client, const str &channelName)
{
	std::string msg = ":localhost 474 ";
    msg += client.getclientnick() + " ";
    msg += channelName + " :Cannot join channel (+b)";
    msg += "\n";
    send(client.getclientsocket(), msg.c_str(), msg.size(), 0);
	return 0;
}

// ERR_CHANNELISFULL (471)
int NumericReplys::rpl_channelisfull(const Client &client, const str &channelName)
{
	std::string msg = ":localhost 471 ";
    msg += client.getclientnick() + " ";
    msg += channelName + " :Cannot join channel (+l)";
    msg += "\n";
    send(client.getclientsocket(), msg.c_str(), msg.size(), 0);
	return 0;
}

// ERR_INVITEONLYCHAN (473)
int NumericReplys::rpl_inviteonlychan(const Client &client, const str &channelName)
{
	std::string msg = ":localhost 473 ";
    msg += client.getclientnick() + " ";
    msg += channelName + " :Cannot join channel (+i)";
    msg += "\n";
    send(client.getclientsocket(), msg.c_str(), msg.size(), 0);
	return 0;
}

// ERR_NOTONCHANNEL (442)
int NumericReplys::rpl_notonchannel(const Client &client, const str &channelName)
{
	std::string msg = ":localhost 442 ";
    msg += client.getclientnick() + " ";
    msg += channelName + " :You're not on that channel";
    msg += "\n";
    send(client.getclientsocket(), msg.c_str(), msg.size(), 0);
	return 0;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */