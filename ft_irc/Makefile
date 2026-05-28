NAME = ircserv

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

INC_DIR = includes
SRC_DIR = srcs
OBJ_DIR = obj

INCLUDES = -I$(INC_DIR) -I$(INC_DIR)/commands

SRCS = $(SRC_DIR)/main.cpp \
       $(SRC_DIR)/Parser.cpp \
       $(SRC_DIR)/Channel.cpp \
       $(SRC_DIR)/Client.cpp \
       $(SRC_DIR)/ClientManager.cpp \
       $(SRC_DIR)/ChannelManager.cpp \
       $(SRC_DIR)/CommandMessage.cpp \
       $(SRC_DIR)/CommandDispatcher.cpp \
       $(SRC_DIR)/ReplyBuilder.cpp \
       $(SRC_DIR)/commands/PassCommand.cpp \
       $(SRC_DIR)/commands/NickCommand.cpp \
       $(SRC_DIR)/commands/UserCommand.cpp \
       $(SRC_DIR)/commands/JoinCommand.cpp \
       $(SRC_DIR)/commands/PartCommand.cpp \
       $(SRC_DIR)/commands/PrivmsgCommand.cpp \
       $(SRC_DIR)/commands/InviteCommand.cpp \
       $(SRC_DIR)/commands/QuitCommand.cpp \
       $(SRC_DIR)/commands/KickCommand.cpp \
       $(SRC_DIR)/commands/TopicCommand.cpp \
       $(SRC_DIR)/commands/ModeCommand.cpp

OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re