#include "../incs/bot.hpp"

int main(int ac, char **av) {

    if (ac == 4)
        Bot myBot(av[1], av[2], av[3]);
    else
        std::cout << "ERROR" << ac << "\n";
    return 0;



    //close(_botL);
}
