NAME = Ircserv
CPPFLAGS = -Werror -Wall -Wextra -std=c++98 -fsanitize=address -g
SRC = main.cpp FDMatcher.cpp IRCServer.cpp 
SRCMSG = Messages/ServerMessage.cpp Messages/tools.cpp Messages/Response.cpp Messages/cmd_topic.cpp Messages/cmd_privmsg.cpp Messages/cmd_mode.cpp Messages/cmd_invite.cpp Messages/cmd_kick.cpp
SRCPRS = Parsing/IRCMessageParse.cpp

OBJ = $(SRC:.cpp=.o) $(SRCMSG:.cpp=.o) $(SRCPRS:.cpp=.o)

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