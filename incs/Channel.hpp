#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <cmath>
# include <string>
# include <vector>
# include <map>
# include <Client.hpp>
# include <sys/socket.h> // send()
# include <sstream> // std::ostringstream
# define BLANK "\033[0m"
# define RED "\033[38;5;1m"
# define PURPLE "\033[38;5;13m"
# define YELLOW "\033[33m"
# define BLUE "\033[38;5;14m"
# define GREEN "\033[38;5;47m"
# define ORANGE "\033[38;5;208m"
typedef std::string	str;

class Channel
{

	public:

		Channel(const str &name, const str &topic);
		Channel( Channel const & src );
		~Channel();
		
		void addUser(const Client &user);
		void sendMessage(const Client &user, const str &message, const str &msgType);
		void sendMessage(str Message);
		void leave(const Client &user, const str &goodbyMessage);
		void topicCommand(const Client &user, const str command);
		// Checkers
		int userIsMemberOfChannel(const str &nickMask) const;
		std::size_t getNumberOfUsers() const;
		bool isChannelOperator(const str &nickMask);
		bool isBanned(const str &nickMask);
		int channelSizeLimit();
		bool wasInvited(const str &nickMask);
		const str getKey();
		bool canSpeak(const str &nickMask);
		bool isChannelSecret();
		bool canChangeTopic(const str &nickMask);

		// Seters
		int addChannelOp(const str &op, const str &newUser);
    	int rmvChannelOp(const str &op, const str &userToRemove);
		int addClientBanned(const str &op, const str &nickMask);
		int rmvClientBanned(const str &op, const str &nickMask);

		// Activate/Deactivate Modes
		void banFlag(const str &nickMask);
		void banExeptionFlag(const str &nickMask);
		void sizeLimitFlag(const str &nickMask, int optionalLimit);
		void inviteFlag(const str &nickMask);
		void inviteExeptionFlag(const str &nickMask);
		void keyFlag(const str &nickMask, const str &key);
		void moderaterFlag(const str &nickMask);
		void secretFlag(const str &nickMask);
		void protectedTopicFlag(const str &nickMask);
		void exeternalMsgFlag(const str &nickMask);

		// Operators
		Channel &		operator=( Channel const & rhs );

		// Accessors
		const std::vector<Client> &getUsers() const;
        const str &getName() const;
        const str &getTopic() const;
		const str &getType() const;
		const std::vector<str> &getChannelOperators() const;
		const std::map<str, std::vector<str> > &getChannelModes() const;

		void delete_users_by_name(str nick);
		void delete_users_by_fd(int fd);
		str get_all_user_nicks();
		void print_all_user_DEBUG(){
		int opt = 0;
		std::vector<Client>::iterator it = _users.begin();
		for (; it != _users.end(); it++)
		{
			std::cout << "####CLIENTS LIST####\n" ;
			std::cout << YELLOW << "[DEBUG](print_all_USER_vector_or_index)" << BLANK << "Index " << opt << " NIck: " << it->getclientnick() << " USER: " << it->getclientuser() << " Realname : " << it->getRealName() << " FD : " << it->getclientsocket() << "\n";
			opt++;
   			}
		};
		bool has_user(int fd) const;
		void update_user_list(int fd);
	private:
		str _channelName;
		str _channelTopic;
		str _channelType;
        std::vector<Client> _users;

		// Channel Operators
		std::vector<str> _channelOperators;

		// Channel Modes Map
		std::map<str, std::vector<str> > _channelModes;

		// Additional fucntions
		void removeFromVector(const Client &user, std::vector<str> &vector);
        void removeUser(const Client &user);
		void changeChannelMode(const str &mode);
		void changeTopic(const Client &user, const str &newTopic);
};

std::ostream &			operator<<( std::ostream & o, Channel const & i );

class channelErrorException : public std::exception {
	private:
		const std::string _errorMessage;
	public: 
		channelErrorException(const std::string errorMessage): _errorMessage(errorMessage) {};
		virtual const char *what() const throw() {
			return _errorMessage.c_str();
		}
		virtual ~channelErrorException() throw() {};
};




#endif /* ********************************************************* CHANNEL_H */