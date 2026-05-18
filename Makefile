NAME	= ircserv

CXX	= c++
CXXFLAGS= -Werror -Wall -Wextra -std=c++98
INCLUDES= -I includes -I includes/commands

SRCS	= srcs/main.cpp \
	srcs/Server.cpp \
	srcs/Client.cpp \
	srcs/ClientManager.cpp \
	srcs/Channel.cpp \
	srcs/ChannelManager.cpp \
	srcs/CommandMessage.cpp \
	srcs/Parser.cpp \
	srcs/CommandDispatcher.cpp \
	srcs/ReplyBuilder.cpp \
	srcs/commands/PassCommand.cpp \
	srcs/commands/NickCommand.cpp \
	srcs/commands/UserCommand.cpp \
	srcs/commands/JoinCommand.cpp \
	srcs/commands/PrivmsgCommand.cpp \
	srcs/commands/PartCommand.cpp \
	srcs/commands/QuitCommand.cpp \
	srcs/commands/KickCommand.cpp \
	srcs/commands/InviteCommand.cpp \
	srcs/commands/TopicCommand.cpp \
	srcs/commands/ModeCommand.cpp \
	srcs/commands/PingCommand.cpp	

OBJS	= $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
