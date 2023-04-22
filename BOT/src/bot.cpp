#include "../incs/bot.hpp"

Bot::Bot(str ServPort, str ServPassword, str Channeltojoin) 
{
    _ServPassword = ServPassword;
    _ServPort = ServPort;
    _ChannelToJoin = Channeltojoin;
    _is_ponged = false;
    createBotSocket();
    connectToServer();
    RegisterInServer();
    MainConnectionLoop();
}

void Bot::createBotSocket()
{
    this->_BotSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_BotSocket < 0) {
        std::cerr << "Error opening socket" << std::endl;
        return;
    }
}

void Bot::connectToServer()
{
    struct addrinfo hints, *result;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo("localhost", _ServPort.c_str(), &hints, &result) != 0) {
        std::cerr << "Error resolving address" << std::endl;
        return ;
    }
    struct sockaddr_in* address = (struct sockaddr_in*)result->ai_addr;
    if (connect(_BotSocket, (struct sockaddr*)address, sizeof(*address)) != 0) {
        std::cerr << "Error connecting to server" << std::endl;
        return ;
    }
}

void Bot::RegisterInServer()
{
    std::string nickname = "MyBot";
    std::string username = "MyBot";
    std::string realname = "My Bot";
    std::ostringstream registration_msg;
    registration_msg << "PASS " << _ServPassword << "\r\n";
    registration_msg << "NICK " << nickname << "\r\n";
    registration_msg << "USER " << username << " 0 * :" << realname << "\r\n";
    send(_BotSocket, registration_msg.str().c_str(), registration_msg.str().size(), 0);
}

void Bot::MainConnectionLoop()
{

    char buffer[1024];
    while (true) 
    {
        int num_bytes = recv(_BotSocket, buffer, sizeof(buffer), 0);
        if (num_bytes <= 0) {
            std::cerr << "Error receiving data from server" << std::endl;
            break;
        }
        else
        {
            HandleServerInput(buffer, num_bytes);
            std::memset(buffer, 0, num_bytes);
        }
    }
}

void Bot::HandleServerInput(char *buffer, int num_bytes)
{
    std::cout << buffer << std::endl;
    str buf(buffer, num_bytes);
    std::cout << "BUFFER:" << "["<< buf << "]" <<std::endl ;
    if (!_is_ponged && buf.find("PING :localhost"))
    {
        _is_ponged = true;
        std::ostringstream PONG_msg;
        PONG_msg << "PONG :localhost\r\n";
        send(_BotSocket, PONG_msg.str().c_str(), PONG_msg.str().size(), 0);
        usleep(100000);
        std::ostringstream join_msg;
        join_msg << "JOIN " << _ChannelToJoin << "\r\n";
        join_msg << "MODE " << _ChannelToJoin << "\r\n";
        join_msg << "WHO " << _ChannelToJoin << "\r\n";
        send(_BotSocket, join_msg.str().c_str(), join_msg.str().size(), 0);
    }
    else if (_is_ponged && buf.find("PRIVMSG ") != std::string::npos)
        HandlePRIVMSG(buf);
    else
        return;
}

void Bot::HandlePRIVMSG(str buf)
{
    str SenderNickMask, nick, message, command, sendernick;
    std::stringstream s(buf);
    std::string response_from_bot;
    s >> SenderNickMask;
    s >> command;
    s >> nick;
    s >> message;
    size_t findcol = SenderNickMask.find(":") + 1;
    size_t findexc = SenderNickMask.find("!") - 1;
    if (findcol == str::npos || findexc == str::npos || nick[0] == '#' )
        return; 
    sendernick = SenderNickMask.substr(findcol, findexc);
    ChatGPT(message,sendernick,response_from_bot);
    str teste = "PRIVMSG " + sendernick + " :" + response_from_bot + "\r\n";
    send(_BotSocket, teste.c_str(), teste.size(), 0);
    std::cout<<"RESPOSTA NO MAIN "<<response_from_bot<<"\n";
}

void Bot::ChatGPT(str message, str sendernick, str &response)
{
    //connect to api
    std::string bearer = TOKEN; 
    std::string args1 = "-d '{ \
    \"model\" : \"text-davinci-003\", \
    \"prompt\" : \" ";
    std::string args2 = " \", \
    \"temperature\": 0.9, \
    \"max_tokens\": 150, \
    \"top_p\": 1, \
    \"frequency_penalty\": 0, \
    \"presence_penalty\": 0.6, \
    \"stop\": [\" Human:\", \" AI:\"] }' \
    ";
std::cout<<"MESSAGE: "+ message + "\n";
std::string request = "curl https://api.openai.com/v1/completions  -H \"Content-Type: application/json\" -H 'Authorization: Bearer " + bearer +"'"+' '+ args1 + message + args2;
std::string output_file = sendernick + ".msg";
std::string final_cmd = request + " > " + output_file;
system(final_cmd.c_str());


//File Handling
std::ifstream response_file(output_file.c_str());
std::string response_raw;
std::getline(response_file, response_raw);

std::string response_send;
std::cout<<response_raw<<"\n";
response_raw.find("\n\n");
response_send= response_raw.substr(response_raw.find("\"text\":") + 8, response_raw.find("index\"") -  response_raw.find("\"text\":") - 11);
int i=0;
for(;i<5;i++){
    if(response_send[i] != '\\' && response_send[i] != 'n')
        break;
}
response_raw = response_send.substr(i,response_send.size()-i);
response_send = response_raw;
std::stringstream stream_1(response_send);
std::string final;
std::string words;
/*while(stream_1 >> words){
    //if(words!=)
    final.append(words);
    final.append(" ");
}*/
std::replace(response_send.begin(),response_send.end(),'\n',' ');

std::cout<<"\n\nFILTERED: "<< response_send <<std::endl;
std::string remove_comand= "rm " + output_file;
//system(remove_comand.c_str());
response = response_send;

}