#ifndef FT_IRC_HPP
#define FT_IRC_HPP


#define RESET " \033[0m"
#define BOLD " \033[1m"
#define RED  "\033[31m"
#define GREEN  "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE  "\033[34m"
#define MAGENTA  "\033[35m"
#define CYAN  "\033[36m"

#include <cctype>
#include <string>
#include <sstream>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>

class ft_irc
{
    public:
        std::string nick;
        std::string user;
        std::string pass;
        std::string port;
        std::string server;
};

bool    check_info(ft_irc irc);
bool    enough_elements(const std::string &input);


int    handle_server(ft_irc irc);


void    welcoming_msg(ft_irc &irc);


#endif