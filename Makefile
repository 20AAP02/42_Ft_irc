CC			=	c++
FLAGS		=	-g -Wall -Wextra -Werror -std=c++98 #-fsanitize=address
RMV			=	rm -rf
NAME		=	ircserv
INPUT		?=

PATH_SRC	=	./src
SRC			=	$(shell find $(PATH_SRC) -name '*.cpp')
INCLUDES	=	./incs

all: $(NAME)

$(NAME): $(SRC)
	@$(CC) $(FLAGS) -I$(INCLUDES) -o $(NAME) $(SRC)
	@$(GREEN) && $(BOLD) && printf "[ $(NAME) built! ]\n" && $(RMVMODES)

clean:

fclean: clean
	@$(RMV) $(NAME)
	@$(RED) && $(DIM) && printf "[ Cleaned Bin! ]\n" && $(RMVMODES)

re: fclean all

.PHONY: all clean fclean re e v cls

# Complementary --------------------------

cls:
	@clear

e: cls re cls
	@./$(NAME) $(INPUT)

v: cls re cls
	@valgrind -s --leak-check=full --show-leak-kinds=all ./$(NAME) $(INPUT)

debug: cls re cls
	@gdb ./$(NAME) $(INPUT)

# COLORS / TEXT MODES

BLACK			=	tput setaf 0
RED				=	tput setaf 1
GREEN			=	tput setaf 2
YELLOW			=	tput setaf 3
BLUE			=	tput setaf 4
MAGENTA			=	tput setaf 5
CYAN			=	tput setaf 6
WHITE			=	tput setaf 7

BOLD			=	tput bold
DIM				=	tput dim
UNDERLINEon		=	tput smul
UNDERLINEoff	=	tput rmul
SATNDOUTon		=	tput smso
SATNDOUToff		=	tput rmso

RMVMODES		=	tput sgr0
