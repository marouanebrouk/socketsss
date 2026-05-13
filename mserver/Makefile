NAME = test_server

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = Server.cpp Client.cpp main.cpp IrcParser.cpp IrcMessage.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(NAME) clean

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
