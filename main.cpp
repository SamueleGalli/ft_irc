#include "ft_irc.hpp"

bool valid_number(const std::string &s)
{
    int num = std::atoi(s.c_str());
    if (num < 0 || num > 65535)
    {
        colored_message("🚨Error: \n(not a valid port number)🚨", RED);
        return false;
    }
    if (s.empty())
    {
        colored_message("🚨Error: \n(no number found)🚨", RED);
        return false;
    }
    for (size_t i = 0; i < s.size(); i++)
    {
        if (!std::isdigit(s[i]))
        {
            colored_message("🚨Error: \n(not a number)🚨", RED);
            return false;
        }
    }
    return true;
}

bool valid_password(const std::string &s)
{
    if (s.empty())
    {
        colored_message("🚨Error: \n(no password found)🚨", RED);
        return (false);
    }
    if (s.size() < 8)
    {
        colored_message("🚨Error: \n(password too short)🚨", RED);
        return (false);
    }
    int up_case = 0;
    int low_case = 0;
    int num = 0;
    int symbol = 0;
    for (size_t i = 0; i < s.size(); i++)
    {
        if (std::isupper(s[i]))
            up_case = 1;
        else if (std::islower(s[i]))
            low_case = 1;
        else if (std::isdigit(s[i]))
            num = 1;
        else if (std::ispunct(s[i]))
            symbol = 1;
    }
    if (up_case == 0)
    {
        colored_message("🚨Error: \n(password must contain at least one uppercase letter)🚨", RED);
        return (false);
    }
    if (low_case == 0)
    {
        colored_message("🚨Error: \n(password must contain at least one lowercase letter)🚨", RED);
        return (false);
    }
    if (num == 0)
    {
        colored_message("🚨Error: \n(password must contain at least one number)🚨", RED);
        return (false);
    }
    if (symbol == 0)
    {
        colored_message("🚨Error: \n(password must contain at least one special character)🚨", RED);
        return (false);
    }
    return (true);
}

int main(int c, char **v)
{
    ft_irc irc;
    irc.client.client_sock = -1;
    if (c != 3)
    {
        colored_message("🚨Error: \n(not valid number of arguments)🚨", RED);
        return (1);
    }
    if (valid_number(v[1]) == false)
        return (1);
    if (valid_password(v[2]) == false)
        return (1);
    irc.port = v[1];
    irc.pass_server = v[2];
    if (handle_server(irc) == 1)
        return (1);
}