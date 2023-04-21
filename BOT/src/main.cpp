#include "../incs/bot.hpp"

int main() {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        std::cerr << "Error opening socket" << std::endl;
        return 1;
    }
    struct addrinfo hints, *result;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(SERVER_ADDRESS.c_str(), SERVER_PORT.c_str(), &hints, &result) != 0) {
        std::cerr << "Error resolving address" << std::endl;
        return 1;
    }
    struct sockaddr_in* address = (struct sockaddr_in*)result->ai_addr;
    if (connect(socket_fd, (struct sockaddr*)address, sizeof(*address)) != 0) {
        std::cerr << "Error connecting to server" << std::endl;
        return 1;
    }
    std::string nickname = "MyBot";
    std::string username = "MyBot";
    std::string realname = "My Bot";
    std::ostringstream registration_msg;
    registration_msg << "PASS " << PASSWORD << "\r\n";
    registration_msg << "NICK " << nickname << "\r\n";
    registration_msg << "USER " << username << " 0 * :" << realname << "\r\n";
    send(socket_fd, registration_msg.str().c_str(), registration_msg.str().size(), 0);


//     [MESSAGE]: MODE #example
//   ->Sent by teste1!teste1q1@localhost[5]
// [MESSAGE]: WHO #example


    bool is_ponged = false;
    char buffer[1024];
    while (true) {
        int num_bytes = recv(socket_fd, buffer, sizeof(buffer), 0);
        if (num_bytes < 0) {
            std::cerr << "Error receiving data from server" << std::endl;
            break;
        }
        if (num_bytes == 0) {
            std::cerr << "Connection closed by server" << std::endl;
            break;
        }
        else if (!is_ponged && str(buffer, num_bytes).find("PING :localhost\r\n"))
        {
            is_ponged = true;
            std::ostringstream PONG_msg;
            PONG_msg << "PONG :localhost\r\n";
            send(socket_fd, PONG_msg.str().c_str(), PONG_msg.str().size(), 0);
            usleep(100000);
            std::ostringstream join_msg;
            join_msg << "JOIN " << CHANNEL << "\r\n";
            join_msg << "MODE " << CHANNEL << "\r\n";
            join_msg << "WHO " << CHANNEL << "\r\n";
            send(socket_fd, join_msg.str().c_str(), join_msg.str().size(), 0);
        }


        buffer[num_bytes] = '\0';
        std::cout << buffer << std::endl;
    }
    close(socket_fd);
    return 0;
}
