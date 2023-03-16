NAME = irc_server

PATH_SRC = ./src

SRC = $(PATH_SRC)/main.cpp 

INCLUDES	=	./incs

OBJ = $(SRC:%.cpp = %.o)

CC = c++

FLAGS = -g -Wall -Wextra -Werror -std=c++98 -fsanitize=address 

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(FLAGS) -I$(INCLUDES) -o $(NAME) $(OBJ) 
	@printf "\033[44m[$(NAME) built!]\033[0m\n"

clean:
	@rm -rf *.opp
	@printf "\033[38;5;1m[Cleaned objects!]\033[0m\n"

fclean: clean
	@rm -rf $(NAME)
	@printf "\033[38;5;1m[Cleaned Bin!]\033[0m\n"

re: fclean all
