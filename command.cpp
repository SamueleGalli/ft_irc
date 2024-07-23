#include "ft_irc.hpp"

/*Nickname Esistente: Se il nickname è già in uso, il server può rispondere con un messaggio di errore e richiedere un nickname diverso.
Comandi Mancanti o Malformati: Se i comandi sono malformati o mancanti, il server invierà messaggi di errore o di richiesta di correzione.*/

// Funzione per gestire il comando PASS
int process_pass_command(ft_irc &irc, bool &authenticated)
{
    if (second_command(irc) == irc.pass_server)
    {
        colored_message("password accepted", GREEN);
        authenticated = true;
        return 0;
    }
    else
    {
        colored_message("🚨Error: \n(invalid password)🚨", RED);
        return 1;
    }
}

void send_error_message(int sock, const std::string &message)
{
    std::string buffer = "ERROR : " + message + "\r\n";
    send(sock, buffer.c_str(), buffer.size(), 0);
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
    std::stringstream ss(second_command(irc));
    ss >> irc.client.user >> irc.client.host >> irc.client.server;
    std::getline(ss, irc.client.realname);
    if (irc.client.realname[0] != ':' || irc.client.user.empty() || irc.client.server.empty())
    {
        send_error_message(irc.client.client_sock, "Missing required information");
        return (1);
    }
    irc.client.realname.erase(0, 1);
    if (irc.client.host.empty() || irc.client.host.length() > 253)
        return (1);
    if (check_server_host(irc.client.host) == 1)
    {
        send_error_message(irc.client.client_sock, "Invalid hostname");
        return (1);
    }
    if (check_server_host(irc.client.server) == 1)
    {
        send_error_message(irc.client.client_sock, "Invalid server name");
        return (1);
    }
    colored_message("USER :" + irc.client.user + "\r\n", YELLOW);
    colored_message("HOST :" + irc.client.host + "\r\n", YELLOW);
    colored_message("SERVER :" + irc.client.server + "\r\n", YELLOW);
    colored_message("REALNAME :" + irc.client.realname + "\r\n", YELLOW);
    return (0);
}

// Funzione per gestire i comandi generali
int handle_command(ft_irc &irc)
{
    static bool authenticated = false;
    if (!authenticated)
    {
        if (first_command(irc) == "PASS")
            return process_pass_command(irc, authenticated);
        if (first_command(irc) == "NICK" || first_command(irc) == "USER")
        {
            send_error_message(irc.client.client_sock, "Not authenticated");
            return 1;
        }
    }
    if (first_command(irc) == "NICK")
    {
        irc.client.nick = second_command(irc);
        colored_message("NICK :" + irc.client.nick + "\r\n", YELLOW);
        return 0;
    }
    if (first_command(irc) == "USER" && handle_user(irc) == 0)
    {
        return (0);
    }
    std::cout << CYAN << "Client: " << RESET;
    colored_message(irc.buffer, CYAN);
    return 0;
}