#include "ft_irc.hpp"

void init_poll(ft_irc &irc)
{
    irc.p_fds[0].events = POLLIN;
    irc.p_fds[0].fd = irc.client.client_sock;
    memset(irc.buffer, 0, sizeof(irc.buffer));
}

void    colored_message(const std::string message, const std::string color)
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
        if (count == 0 && (word != "NICK" && word != "USER"))
            return (false);
        count++;
    }
    return (count == 2);
}

bool    check_info(ft_irc irc)
{
    return (enough_elements(irc.client.nick) && enough_elements(irc.client.user));
}

std::string first_command(ft_irc irc)
{
    std::string command;
    std::stringstream ss(irc.buffer);
    ss >> command;
    return (command);
}

std::string second_command(ft_irc irc)
{
    std::string first_word;
    std::stringstream ss(irc.buffer);

    ss >> first_word;

    std::string rest_of_command;
    std::getline(ss, rest_of_command);

    if (!rest_of_command.empty() && rest_of_command[0] == ' ')
        rest_of_command.erase(0, 1);
    if (!rest_of_command.empty() && rest_of_command[0] == ':')
        rest_of_command.erase(0, 1);
    return rest_of_command;
}