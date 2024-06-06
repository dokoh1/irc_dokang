NAME = Ircserv
CPPFLAGS = -Werror -Wall -Wextra -std=c++98
SRC = main.cpp FDMatcher.cpp IRCServer.cpp
OBJ = $(SRC:.cpp=.o)

all : $(NAME)
$(NAME) : $(OBJ)
	$(CXX) $(CPPFLAGS) $(OBJ) -o $@

%.o : %.c
	$(CXX) $(CPPFLAGS) -c $< -o $@

clean :
	rm -rf $(OBJ)

fclean :
	rm -rf $(OBJ)
	rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re