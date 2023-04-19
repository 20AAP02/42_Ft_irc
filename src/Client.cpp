#include <Client.hpp>

typedef std::string str;

Client::Client(str nick, int socket_fd) : _nick(nick), _socket_fd(socket_fd)
{
    this->_is_admin = false;
    this->_is_logged_in = false;
    this->_password = false;
    this->_nick_cl = false;
    this->_user_cl = false;
    this->_password = false;
    this->is_waiting_for_pong = false;
};

Client::Client(str nick, str user, int socket_fd) : _nick(nick), _user(user), _socket_fd(socket_fd)
{
    this->_is_admin = false;
};

Client::~Client(){};

const str Client::getclientnick() const
{
	// std::cout << "SERVER PRINT: nick -> (" << this->_nick << ")\n";
	return this->_nick;
};

/*void Client::set_pollfd_fd(int in){
    this->client_pollfd.fd = in;
};
void Client::set_pollfd_revents(int in){
     this->client_pollfd.revents = in;
};
void Client::set_pollfd_events(int in){
    this->client_pollfd.events = in;
};

struct pollfd Client::get_client_poll(){
    struct pollfd aux = client_pollfd;
    return client_pollfd;
}
*/

void Client::ping_client()
{
    str msg = "PING :localhost\r\n";
	send(this->getclientsocket(), msg.c_str(), msg.size(), 0);
    this->_time_ping = time(NULL);
    this->is_waiting_for_pong = true;
}

const str Client::getclientuser() const
{
	return this->_user;
};

const str &Client::getmessage() const
{
    return this->_message;
};

bool Client::is_admin() const
{
    return this->_is_admin;
};

bool Client::is_logged_in() const{
    return this->_is_logged_in;
}

bool Client::get_pass_bool() {
    return this->_password;
}
bool Client::get_nick_bool(){
    return this->_nick_cl;
}
bool Client::get_user_bool() {
    return this->_user_cl;
}

long Client::get_time_ping()
{
    return this->_time_ping;
}

int Client::getclientsocket() const
{
    return this->_socket_fd;
}
void Client::set_message(str message)
{
    this->_message = message;
}

void Client::set_admin(bool switch_admin)
{
    this->_is_admin = switch_admin;
}

void Client::set_logged()
{
    this->_is_logged_in = true;
}


void Client::set_pass_bool()
{
    this->_password = true;
}
void Client::set_nick_bool()
{
    this->_nick_cl = true;
}
void Client::set_user_bool()
{
    this->_user_cl = true;
}

str Client::getRealName()
{
    return (this->_realname);
}

void Client::setnick(str new_nick)
{
    this->_nick = new_nick;
}

void Client::setuser(str new_user)
{
    this->_user = new_user;
}

void Client::setrealname(str realname)
{
    this->_realname = realname;
}

str Client::getNickmask() const {
    str nickmask;

	nickmask.append(this->getclientnick());
    nickmask.append("!");
	nickmask.append(this->getclientuser());
    nickmask.append("@");
    nickmask.append("localhost");
    return nickmask;

}

 void Client::sendPrivateMsg(const Client& recipient, const str& message, const str &type) const
 {
	std::string msg = ":" + this->getclientnick() + "!~" + this->getNickmask() + " " + type + " " + message + "\n";
	send(recipient.getclientsocket(), msg.c_str(), msg.size(), 0);
 }


