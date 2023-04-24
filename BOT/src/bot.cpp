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
    if (_BotSocket < 0)
    {
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
    if (getaddrinfo("localhost", _ServPort.c_str(), &hints, &result) != 0)
    {
        std::cerr << "Error resolving address" << std::endl;
        return;
    }
    struct sockaddr_in *address = (struct sockaddr_in *)result->ai_addr;
    if (connect(_BotSocket, (struct sockaddr *)address, sizeof(*address)) != 0)
    {
        std::cerr << "Error connecting to server" << std::endl;
        return;
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
        if (num_bytes <= 0)
        {
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
    str buf(buffer, num_bytes);
    //std::cout << "BUFFER:"
         //     << "[" << buf << "]" << std::endl;

    if (buf.find("PING :localhost") != std::string::npos)
    {
        str PONG_msg("PONG :localhost\r\n");
        send(_BotSocket, PONG_msg.c_str(), PONG_msg.size(), 0);
    }

    if (!_is_ponged)
    {
        _is_ponged = true;

        std::string _msg;
        _msg = "JOIN " + _ChannelToJoin + "\r\n";
        send(_BotSocket, _msg.c_str(), _msg.size(), 0);
        _msg = "MODE " + _ChannelToJoin + "\r\n";
        send(_BotSocket, _msg.c_str(), _msg.size(), 0);
        _msg = "WHO " + _ChannelToJoin + "\r\n";
        send(_BotSocket, _msg.c_str(), _msg.size(), 0);
        str teste = "PRIVMSG ";
        teste += "#public";
        teste += " :";
        teste += "I'll be your assistant, anything you need send me a private message!\r\n";
        send(_BotSocket, teste.c_str(), teste.size(), 0);
        str PONG_msg("PONG :localhost\r\n");
        send(_BotSocket, PONG_msg.c_str(), PONG_msg.size(), 0);
    }
    else if (_is_ponged && buf.find("PRIVMSG ") != std::string::npos)
        HandlePRIVMSG(buf);
    else
        return;
}

void Bot::HandlePRIVMSG(str buf)
{
    str SenderNickMask, nick, message, command, sendernick, word;
    std::stringstream s(buf);
    std::string response_from_bot;
    s >> SenderNickMask;
    s >> command;
    s >> nick;
    while (s >> word)
        message += word + ' ';

    size_t findcol = SenderNickMask.find(":") + 1;
    size_t findexc = SenderNickMask.find("!") - 1;
    if (findcol == str::npos || findexc == str::npos || nick[0] == '#')
        return;
    sendernick = SenderNickMask.substr(findcol, findexc);
    ChatGPT(message, sendernick, response_from_bot);
    std::cout <<  RED "HUMAN :[" << message << "]\n" BLANK;
    str teste = "PRIVMSG " + sendernick + " :" + response_from_bot + "\r\n";
    send(_BotSocket, teste.c_str(), teste.size(), 0);
    std::cout << GREEN "BOT :[" << response_from_bot << "]\n" BLANK;

}

void Bot::handlehistory(str sendernick,str &message, std::ofstream &history_conversation_out)
{
    std::ifstream history_conversation;
    std::string history_file_name = sendernick + ".history";
    str history_string;
    str aux_string;
    history_conversation.open(history_file_name.c_str());
    if (!history_conversation)
        history_conversation.open(history_file_name.c_str());
    else
    {
        while (std::getline(history_conversation, aux_string)){
            history_string += aux_string;
        history_string.erase(history_string.end() - 1, history_string.end());
        history_string += "\\n";
        }
    }
    //std::cout << "AQUI e o historico [" << history_string << "]\n";
    history_conversation.close();
    //std::ofstream history_conversation_out;
    history_conversation_out.open(history_file_name.c_str(), std::ios_base::app);
    message.erase(message.begin(), message.begin() + 1);
    message.erase(message.end() - 1, message.end());
    //std::cout << "Mensagem que entra na funcao: " << message << std::endl;
    history_conversation_out<<"Human:" + message + "\\n";
    //return history_conversation_out;
}

void Bot::makeAPIrequest(str output_file, str message)
{
     std::string bearer = TOKEN;
     std::string args1 = "-d '{ \
     \"model\" : \"text-davinci-003\", \
     \"prompt\" : \" ";
     std::string args2 = " \", \
     \"temperature\": 0.1, \
     \"max_tokens\" : 200, \
     \"top_p\": 1, \
     \"frequency_penalty\": 0, \
     \"presence_penalty\": 0.6, \
     \"stop\": [\" Human:\", \" AI:\"] }' \
     ";
    //std::cout<<"MESSAGE: "+ history_string + "\n";
    std::string request = "curl https://api.openai.com/v1/completions  -H \"Content-Type: application/json\" -H 'Authorization: Bearer " + bearer +"'"+' '+ args1 + message + args2;
    std::string final_cmd = request + " > " + output_file;
    system(final_cmd.c_str());
}

str Bot::HandleResponse(str output_file,std::ofstream &history_conversation_out)
{
    std::ifstream response_file(output_file.c_str());
    std::string response_raw;
    std::getline(response_file, response_raw);
    str response_send;
    //std::cout<<response_raw<<"\n";
    response_send= response_raw.substr(response_raw.find("\"text\":") + 8, response_raw.find("index\"") -  response_raw.find("\"text\":") - 11);
    std::replace(response_send.begin(),response_send.end(),'\n',' ');
   // std::cout<<"\n\nFILTERED NOT TREATED: "<< response_send <<std::endl;
    response_send = remove_from_str("\\n", response_send);
    history_conversation_out << "AI:"+response_send + "\\n";
    //std::cout<<"\n\nFILTERED: "<< response_send <<std::endl;
    std::string remove_comand= "rm " + output_file;
    //system(remove_comand.c_str());
    return response_send;
}

void Bot::ChatGPT(str message, str sendernick, str &response)
{
    if (!sendernick.size() || !message.size())
        return;

    // Verify if exist history conversation
    std::ofstream history_conversation_out ;
    handlehistory(sendernick, message, history_conversation_out);
    std::string output_file = sendernick + ".msg";
    // connect to api
    makeAPIrequest(output_file, message);
    //File Handling
    response = HandleResponse(output_file, history_conversation_out);
}

str Bot::remove_from_str(str expr, str string_)
{
    str aux_string(string_);
    str aux_string2;
    str aux_string3;
    while (aux_string.find(expr) != str::npos)
    {
        aux_string2 = aux_string.substr(0, aux_string.find(expr));
        aux_string3 = aux_string.substr(aux_string.find(expr) + expr.size(), aux_string.size() - expr.size() - aux_string.find(expr));
        aux_string = aux_string2 + aux_string3;
    }
    return aux_string;
}