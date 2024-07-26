#include "ft_irc.hpp"

void send_error_message(int sock, const std::string &message)
{
    std::string msg_error = "ERROR " + message + "\r\n";
    send(sock, msg_error.c_str(), msg_error.size(), 0);
}
void welcome_msg(ft_irc &irc)
{
    std::string buffer;

    buffer = ":server 001 " + irc.client.nick + " :Welcome to the Internet Relay Network " + irc.client.nick + "!" + irc.client.user + "@" + irc.client.host + irc.client.realname +"\r\n";
    send(irc.client.client_sock, buffer.c_str(), buffer.length(), 0);

    buffer = ":server 002 " + irc.client.nick + " :Your host is " + irc.client.server + ", running version 1.0.0\r\n";
    send(irc.client.client_sock, buffer.c_str(), buffer.length(), 0);

    buffer = ":server 003 " + irc.client.nick + " :This server was created on " + __DATE__ + " at " + __TIME__ + "\r\n";
    send(irc.client.client_sock, buffer.c_str(), buffer.length(), 0);

    buffer = ":server 004 " + irc.client.nick + " " + irc.client.server + " 1.0.0 ao mtov\r\n";
    send(irc.client.client_sock, buffer.c_str(), buffer.length(), 0);
}

int check_server_host(std::string str)
{
    if (str[0] == '-' || str[0] == '.')
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!std::isalnum(str[i]) && str[i] != '-' && str[i] != '.' && str[i] != '*')
            return (1);
    }
    return (0);
}

//controllare
int handle_user(ft_irc &irc)
{
    std::string error_msg;
    error_msg = ":server 461 " + irc.client.nick + " :Not enough parameters\r\n";
    std::stringstream ss(second_command(irc));
    ss >> irc.client.user >> irc.client.host >> irc.client.server;
    std::getline(ss, irc.client.realname);
    if (irc.client.user.empty() || irc.client.server.empty() || irc.client.host.empty() || irc.client.realname.empty())
    {
        send_error_message(irc.client.client_sock, error_msg);
        return (1);
    }
    if (irc.client.realname[1] != ':')
    {
        send_error_message(irc.client.client_sock, error_msg);
        return (1);
    }
    //irc.client.realname.erase(1, 1);
    if (irc.client.host.length() > 253 || check_server_host(irc.client.host) == 1)
    {
        send_error_message(irc.client.client_sock, error_msg);
        return (1);
    }
    if (check_server_host(irc.client.server) == 1)
    {
        send_error_message(irc.client.client_sock, error_msg);
        return (1);
    }
    return (0);
}

int check_nick(const std::string &nick, ft_irc &irc)
{
    std::string error_msg;
    if (nick.length() > 9)
    {
        error_msg = ":server 432 " + nick + " :Erroneous nickname\r\n";
        send_error_message(irc.client.client_sock, error_msg);
        return (1);
    }
    for (unsigned int i = 0; i < nick.length(); i++)
    {
        if (!std::isalnum(nick[i]) && nick[i] != '-' && nick[i] != '.')
        {
            error_msg = ":server 432 " + nick + " :Erroneous nickname\r\n";
            send_error_message(irc.client.client_sock, error_msg);
            return (1);
        }
    }
    return (0);
}