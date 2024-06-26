NAME = Ircserv
CPPFLAGS = -Werror -Wall -Wextra -std=c++98 -g -fsanitize=address
SRC = main.cpp FDMatcher.cpp IRCServer.cpp 
SRCMSG = Messages/IRCMessageParse.cpp Messages/ServerMessage.cpp Messages/tools.cpp Messages/Response.cpp Messages/rpl.cpp
SRCCMD = command/cmd_topic.cpp command/cmd_privmsg.cpp command/cmd_mode.cpp command/cmd_invite.cpp command/cmd_kick.cpp command/cmd_quit.cpp

OBJ = $(SRC:.cpp=.o) $(SRCMSG:.cpp=.o) $(SRCCMD:.cpp=.o)

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