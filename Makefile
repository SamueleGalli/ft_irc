CPP = c++

CPPFLAGS = -std=c++98 -Wall -Werror -Wextra

NAME = ircserv

SRCS = main.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CPP) $(CPPFLAGS) -o $(NAME) $(OBJS)


%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(NAME)

re: fclean all

fclean: clean
	rm -f $(NAME)