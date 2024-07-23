#ifndef FT_IRC_HPP
#define FT_IRC_HPP


#define RESET "\033[0m"
#define RED  "\033[31m"
#define GREEN  "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN  "\033[36m"


#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <cerrno>
#include <cstring>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <cstdio>

class client_info
{
    public:
        std::string nick;
        std::string user;
        int client_sock;
        struct sockaddr_in client_addr;
        socklen_t client_len;
        std::string host;
        std::string server;
        std::string realname;
};

class server_info
{
    public:
        int server_sock;
        struct sockaddr_in server_addr;
};

class ft_irc
{
    public:
        std::string port;
        std::string pass_server;
        struct pollfd p_fds[1];
        server_info server;
        client_info client;
        char buffer[512];
};

bool    check_info(ft_irc irc);
bool    enough_elements(const std::string &input);

int     handle_server(ft_irc &irc);
int     handle_client(ft_irc &irc);
int     handle_command(ft_irc &irc);

std::string first_command(ft_irc irc);
std::string second_command(ft_irc irc);

void    message(const std::string message);
void    init_poll(ft_irc &irc);
void    colored_message(const std::string message, const std::string color);


#endif