#include "../incs/bot.hpp"

int main(int ac, char **av) {

    if (ac == 4)
        Bot myBot(av[1], av[2], av[3]);
    else
        std::cout << RED "Error-> Usage:\n\t./ircBOT <Server Port> <Server Password> <Channel for Bot>\n" BLANK;
    return 0;
}
