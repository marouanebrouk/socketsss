NAME = ircserv

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g

SRCS = main.cpp srcs/Server.cpp srcs/Client.cpp srcs/Channel.cpp srcs/Command.cpp srcs/server_commands.cpp srcs/helpers.cpp $(wildcard cmds/*.cpp)
OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

.PHONY: all clean fclean re

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
