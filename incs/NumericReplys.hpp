#ifndef NUMERICREPLYS_HPP
# define NUMERICREPLYS_HPP

# include <iostream>
# include <string>
# include <Client.hpp>
# include <Channel.hpp>
# include <sys/socket.h> // send()

class NumericReplys
{

	public:

		NumericReplys();
		~NumericReplys();

		// RPL_WELCOME (001)
		int rpl_welcome(const Client &client);

		// RPL_NOTOPIC (331)
		int rpl_notopic(const Client &client, const str channelName);

		// RPL_TOPIC (332)
		int rpl_topic(const Client &client, const str channelName, const str &topic);

		// RPL_NAMREPLY (353)
		int rpl_namreply(const Client &client, const str &channel);

		// RPL_ENDOFNAMES (366)
		int rpl_endofnames(const Client &client, const str &channelName);

		// ERR_CHANOPRIVSNEEDED (482)
		int rpl_chanoprivsneeded(const Client &client, const str &channelName);
		
		// ERR_BADCHANNELKEY (475)
		int rpl_badchannelkey(const Client &client, const str &channelName);

		// ERR_BANNEDFROMCHAN (474)
		int rpl_bannedfromchan(const Client &client, const str &channelName);

		// ERR_CHANNELISFULL (471)
		int rpl_channelisfull(const Client &client, const str &channelName);

		// ERR_INVITEONLYCHAN (473)
		int rpl_inviteonlychan(const Client &client, const str &channelName);

		// ERR_NOTONCHANNEL (442)
		int rpl_notonchannel(const Client &client, const str &channelName);

		//ERR_WRONG_CMD (431)
		int rpl_wrongcmd(const Client &client, const str &channelName);

	private:

};

#endif /* *************************************************** NUMERICREPLYS_H */