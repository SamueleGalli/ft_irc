#include "ft_irc.hpp"

void    print_message(const std::string message, const char *color)
{
    std::cout << color << message << RESET <<std::endl;
}


bool enough_elements(const std::string &input)
{
    std::istringstream iss(input);
    std::string word;
    int count = 0;

    while (iss >> word)
    {
        if (count == 0 && (word != "NICK" && word != "USER" && word !="PASS"))
            return (false);
        count++;
    }
    return (count == 2);
}

bool    check_info(ft_irc irc)
{
    if (enough_elements(irc.client.nick) == false || enough_elements(irc.client.user) == false || enough_elements(irc.client.pass) == false)
        return (false);
    return (true);
}

void welcoming_msg(ft_irc &irc)
{
    int i_n = 5;
    int i_u = 5;
    int i_p = 5;

    while (irc.client.nick[i_n] == ' ' && irc.client.nick[i_n] != '\0')
        i_n++;
    while (irc.client.user[i_u] == ' ' && irc.client.user[i_u] != '\0')
        i_u++;
    while (irc.client.pass[i_p] == ' ' && irc.client.pass[i_p] != '\0')
        i_p++;

    std::string nickname = irc.client.nick.substr(i_n);
    std::string username = irc.client.user.substr(i_u);
    std::string password = irc.client.pass.substr(i_p);

    irc.client.nick = nickname;
    irc.client.user = username;
    irc.client.pass = password;

    print_message("Your provided information are correct. Here's a little recap:", GREEN);
              std::cout << MAGENTA << "\nNICK: (" << irc.client.nick << ")" << RESET
              << YELLOW << "\nUSER: (" << irc.client.user << ")" << RESET
              << CYAN << "\nPASS: (" << irc.client.pass << ")" << RESET
              << GREEN << "\nWELCOME!!" << RESET << std::endl;
}

