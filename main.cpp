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
        print_message("🚨Error: \n(not valid number of arguments)🚨", RED);
        return (1);
    }
    if (valid_number(v[1]) == false)
    {
        print_message("🚨Error: \n(not a number/invalid number)🚨", RED);
        return (1);
    }
    ft_irc irc;
    irc.port = v[1];
    irc.pass_server = v[2];
    if (handle_server(irc) == 1)
        return (1);
}