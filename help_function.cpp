#include "ft_irc.hpp"

ssize_t receive_data(ft_irc &irc)
{
    memset(irc.buffer, 0, sizeof(irc.buffer));
    ssize_t bytes_received = recv(irc.client.client_sock, irc.buffer, sizeof(irc.buffer) - 1, 0);
    if (bytes_received > 0)
        irc.buffer[bytes_received - 1] = '\0'; // Terminazione della stringa
    return bytes_received;
}

void    print_message(const std::string message, const std::string color, ft_irc irc)
{
    if (irc.client.client_sock != -1)
    {
        std::string message_to_send = message + "\r\n";
        send(irc.client.client_sock, message_to_send.c_str(), message_to_send.size(), 0);
    }
    else
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

void welcoming_msg(ft_irc &irc)
{
    int i_n = 5; // Skip "NICK "
    int i_u = 5; // Skip "USER "

    while (irc.client.nick[i_n] == ' ' && irc.client.nick[i_n] != '\0')
        i_n++;
    while (irc.client.user[i_u] == ' ' && irc.client.user[i_u] != '\0')
        i_u++;

    std::string nickname = irc.client.nick.substr(i_n);
    std::string username = irc.client.user.substr(i_u);

    irc.client.nick = nickname;
    irc.client.user = username;

    print_message("Your provided information is correct. Here's a little recap:", GREEN, irc);
    print_message("NICK: " + irc.client.nick, GREEN, irc);
    print_message("USER: " + irc.client.user, GREEN, irc);
    print_message("Welcome " + irc.client.nick + " to the server", GREEN, irc);

    std::string welcome_msg = ":server 001 " + irc.client.nick + " :Welcome to the IRC server\n";
    send(irc.client.client_sock, welcome_msg.c_str(), welcome_msg.size(), 0);
}
