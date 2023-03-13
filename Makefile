CC			= 	clang++ -std=c++98
CFLAGS		= 	-Wall -Wextra -Werror
RM			= 	/bin/rm -f
NAME		= 	ircserv

INCLUDES	=	./
SRCS		=	$(shell find ./ -name '*.cpp')

all: $(NAME)

$(NAME): $(SRCS)
	@$(CC) $(CFLAGS) -I$(INCLUDES) $(SRCS) -o $(NAME)

clean:

fclean: clean
	@$(RM) $(NAME)

re: fclean all

# Complementation

cls:
	@clear

e: cls re cls
	@./$(NAME)

v: cls re cls
	@valgrind ./$(NAME)

.PHONY: all clean fclean re e cls v