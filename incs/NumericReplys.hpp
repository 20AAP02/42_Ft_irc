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
		void rpl_welcome(const Client &client);

		// RPL_NOTOPIC (331)
		void rpl_notopic(const Client &client, const str channelName);

		// RPL_TOPIC (332)
		void rpl_topic(const Client &client, const str channelName, const str &topic);

		// RPL_NAMREPLY (353)
		void rpl_namreply(const Client &client, const Channel &channel);

		// RPL_ENDOFNAMES (366)
		void rpl_endofnames(const Client &client, const str &channelName);

		// ERR_CHANOPRIVSNEEDED (482)
		void rpl_chanoprivsneeded(const Client &client, const str &channelName);

		// Cant enter channel messages --------------------------------------------------
		// ERR_BADCHANNELKEY (475), ERR_BANNEDFROMCHAN (474), ERR_CHANNELISFULL (471), ERR_INVITEONLYCHAN (473)

		void rpl_cannotEnterChannel(const Client &client, const str &channelName);

	private:

};

#endif /* *************************************************** NUMERICREPLYS_H */