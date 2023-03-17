#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <sys/socket.h> // setsockopt(), etc.
# include <netinet/in.h> // INADDR_ANY, etc.
# include <cstdlib> // EXIT_FAILURE, etc.
# include <unistd.h> // read()

/*			COLOURS		*/

# define BLANK "\033[0m"
# define RED "\033[38;5;1m"
# define PURPLE "\033[38;5;13m"
# define YELLOW "\033[33m"
# define BLUE "\033[38;5;14m"
# define GREEN "\033[38;5;47m"
# define ORANGE "\033[38;5;208m"

#endif
