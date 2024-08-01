#include "../header/ft_irc.hpp"

void send_error_message(std::string err_code, const std::string &message, int sock)
{
    std::string msg_error = "server " + err_code + " " + message + "\r\n";
    ssize_t bytes_sent = send(sock, msg_error.c_str(), msg_error.size(), 0);

    if (bytes_sent < 0)
    {
        perror("send");
        std::cerr << RED << "Error sending message: " << msg_error << RESET << std::endl;
    }
}
void welcome_msg(ft_irc &irc, int i)
{
    std::string buffer;

    buffer = ":server 001 " + irc.client[i].nick + " :Welcome to the Internet Relay Network " + irc.client[i].nick + "!" + irc.client[i].user + "@" + irc.client[i].host + irc.client[i].realname +"\r\n";
    send(irc.client[i].client_sock, buffer.c_str(), buffer.length(), 0);

    buffer = ":server 002 " + irc.client[i].nick + " :Your host is " + irc.client[i].server + ", running version 1.0.0\r\n";
    send(irc.client[i].client_sock, buffer.c_str(), buffer.length(), 0);

    buffer = ":server 003 " + irc.client[i].nick + " :This server was created on " + __DATE__ + " at " + __TIME__ + "\r\n";
    send(irc.client[i].client_sock, buffer.c_str(), buffer.length(), 0);

    buffer = ":server 004 " + irc.client[i].nick + " " + irc.client[i].server + " 1.0.0 ao mtov\r\n";
    send(irc.client[i].client_sock, buffer.c_str(), buffer.length(), 0);
}

int is_valid_hostname(const std::string &str)
{
    if (str == "*")
        return 0;
    if (!std::isalnum(str[0]) || !std::isalnum(str[str.length() - 1]))
        return 1;
    for (size_t i = 0; i < str.size(); ++i)
    {
        char c = str[i];
        if (!std::isalnum(c) && c != '-' && c != '.')
            return 1;
        if (c == '-')
        {
            if (i == 0 || i == str.size() - 1 || str[i - 1] == '-' || str[i + 1] == '-')
                return 1;
        }
        if (c == '.')
        {
            if (i == 0 || i == str.size() - 1 || str[i - 1] == '.' || str[i + 1] == '.')
                return 1;
        }
    }
    return true;
}

//controllare
int handle_user(ft_irc &irc, int i)
{
    std::string error_msg;
    error_msg = first_command(irc) + " :Not enough parameters\r\n";
    std::stringstream ss(second_command(irc));
    ss >> irc.client[i].user >> irc.client[i].host >> irc.client[i].server;
    std::getline(ss, irc.client[i].realname);
    if (irc.client[i].user.empty() || irc.client[i].server.empty() || irc.client[i].host.empty() || irc.client[i].realname.empty())
    {
        send_error_message("461", error_msg, irc.client[i].client_sock);
        return (1);
    }
    if (irc.client[i].realname[1] != ':')
    {
        send_error_message("461", error_msg, irc.client[i].client_sock);
        return (1);
    }
    if (irc.client[i].host.length() > 253 || is_valid_hostname(irc.client[i].host) == 1)
    {
        send_error_message("461", error_msg, irc.client[i].client_sock);
        return (1);
    }
    if (is_valid_hostname(irc.client[i].server) == 1)
    {
        send_error_message("461", error_msg, irc.client[i].client_sock);
        return (1);
    }
    if (irc.client[i].registered == 2 || irc.client[i].registered == 1)
        irc.client[i].registered++;
    return (0);
}
int check_nick(const std::string &nick, ft_irc &irc, int i)
{
    if (i > 1)
    {
        for (std::vector<client_info>::const_iterator it = irc.client.begin() + i + 1; it != irc.client.end() + i + 1; ++it)
        {
            if (it->nick == nick)
            {
                            send_error_message("433", nick + " :Nickname is already in use", irc.client[i].client_sock);
                return (1);
            }
        }
    }
    if (second_command(irc) == "no")
    {
        send_error_message("431", ": no nickname given", irc.client[i].client_sock);
        return (1);
    }
    std::string error_msg;
    if (nick.length() > 9)
    {
        error_msg = nick + " :Erroneous nickname\r\n";
        send_error_message("432", error_msg, irc.client[i].client_sock);
        return (1);
    }
    for (unsigned int i = 0; i < nick.length(); i++)
    {
        if (!std::isalnum(nick[i]) && nick[i] != '-' && nick[i] != '.')
        {
            error_msg = nick + " :Erroneous nickname\r\n";
            send_error_message("432", error_msg, irc.client[i].client_sock);
            return (1);
        }
    }
    if (irc.client[i].registered == 1 || irc.client[i].registered == 2)
        irc.client[i].registered++;
    return (0);
}