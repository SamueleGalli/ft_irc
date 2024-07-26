#include "ft_irc.hpp"

/*Nickname Esistente: Se il nickname è già in uso, il server può rispondere con un messaggio di errore e richiedere un nickname diverso.
Comandi Mancanti o Malformati: Se i comandi sono malformati o mancanti, il server invierà messaggi di errore o di richiesta di correzione.*/

// Funzione per gestire il comando PASS
int process_pass_command(ft_irc &irc, bool &authenticated)
{
    if (second_command(irc) == irc.pass_server)
    {
        authenticated = true;
        return 0;
    }
    else
    {
        std::string msg_error = ":server 464 " + irc.client.nick + " :Password incorrect\r\n";
        send_error_message(irc.client.client_sock, msg_error);
        return 1;
    }
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
            send_error_message(irc.client.client_sock, ": Not authenticated");
            return 1;
        }
        else
            send_error_message(irc.client.client_sock, ": Invalid command");
    }
    if (first_command(irc) == "NICK" && check_nick(second_command(irc), irc) == 0)
    {
        irc.client.nick = second_command(irc);
        colored_message("NICK :" + irc.client.nick + "\r\n", YELLOW);
        return 0;
    }
    if (first_command(irc) == "USER" && handle_user(irc) == 0)
    {
        welcome_msg(irc);
        return (0);
    }
    std::cout << CYAN << "Client: " << RESET;
    colored_message(irc.buffer, CYAN);
    return 0;
}