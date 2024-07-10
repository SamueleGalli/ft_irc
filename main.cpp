#include "ft_irc.hpp"

bool valid_number(const std::string &s)
{
    if (s.empty())
        return false;
    for (size_t i = 0; i < s.size(); i++)
    {
        if (!std::isdigit(s[i]))
            return false;
    }
    return true;
}

int main(int c, char **v)
{
    if (c != 3)
    {
        std::cout << RED << "Error: \n(not valid number of arguments)\n" << RESET;
        return (1);
    }
    if (valid_number(v[1]) == false)
    {
        std::cout << RED << "Error: \n(not a number/invalid number)\n" << RESET;
        return (1);
    }
    ft_irc irc;
    irc.port = v[1];
    irc.server = v[2];
    if (handle_server(irc) == 1)
        return (1);
}