#include "ft_irc.hpp"

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
    if (enough_elements(irc.nick) == false || enough_elements(irc.user) == false || enough_elements(irc.pass) == false)
        return (false);
    return (true);
}

void welcoming_msg(ft_irc &irc)
{
    int i_n = 5;
    int i_u = 5;
    int i_p = 5;

    while (irc.nick[i_n] == ' ' && irc.nick[i_n] != '\0')
        i_n++;
    while (irc.user[i_u] == ' ' && irc.user[i_u] != '\0')
        i_u++;
    while (irc.pass[i_p] == ' ' && irc.pass[i_p] != '\0')
        i_p++;

    std::string nickname = irc.nick.substr(i_n);
    std::string username = irc.user.substr(i_u);
    std::string password = irc.pass.substr(i_p);

    irc.nick = nickname;
    irc.user = username;
    irc.pass = password;

    std::cout << GREEN << "Your provided information are correct. Here's a little recap:" << RESET
              << MAGENTA << "\nNICK: (" << irc.nick << ")" << RESET
              << YELLOW << "\nUSER: (" << irc.user << ")" << RESET
              << CYAN << "\nPASS: (" << irc.pass << ")" << RESET
              << GREEN << "\nWELCOME!!" << RESET << std::endl;
}


