#include "../header/ft_irc.hpp"

void    commands(ft_irc &irc, int i)
{
    if (first_command(irc) == "QUIT")
        quit_command(irc, i);
    else
        send_error_message(irc, i, "ops", "invalid command", irc.client[i].client_sock);
    //comandi se registrato
    //comando non riconosciuto devi essere registrato
    /*421     ERR_UNKNOWNCOMMAND
                    "<command> :Unknown command"*/
}

/*c2r3p10% nc 127.0.0.1 6667
PASS Ciao123!
NICK
:unknown 431 * : no nickname given
USER s s s :
n
:unknown ops * invalid command
NICK s1
:server 001 s1 :Welcome to the Internet Relay Network s1!s@s :
:server 002 s1 :Your host is s, running version 1.0.0
:server 003 s1 :This server was created on Aug  5 2024 at 12:19:06
:server 004 s1 s 1.0.0 ao mtov
QUIT esco
:unknown ops * invalid command
QUIT esco
:s1!s@s QUIT esco
*/
int registretion(ft_irc &irc, int i)
{
    if (!irc.client[i].authenticated)
    {
        if (first_command(irc) == "PASS" && irc.client[i].is_pass == false)
            process_pass_command(irc, i);
        else if (first_command(irc) == "PASS" && irc.client[i].is_pass == true)
        {
            send_error_message(irc, i, "462", ": You are already registered", irc.client[i].client_sock);
            return (0);
        }
        else if (first_command(irc) == "NICK")
            nick_command(irc, i);
        else if (first_command(irc) == "USER")
            user_command(irc, i);
        else
            send_error_message(irc, i, "ops", "invalid command", irc.client[i].client_sock);
        if (irc.client[i].is_nick == true && irc.client[i].is_pass == true && \
        irc.client[i].is_user == true)
        {
            welcome_msg(irc, i);
            irc.client[i].authenticated = true;
        }
        std::cout << "--------------------------\nis_nick: " << irc.client[i].is_nick << std::endl;
        std::cout << "is_user: " << irc.client[i].is_user << std::endl;
        std::cout << "is_pass: " << irc.client[i].is_pass << std::endl;
    }
    else
    {

        std::cout << CYAN <<" Client[" << i << "]: " << RESET;
        colored_message(irc.buffer, CYAN);
        if (first_command(irc) == "NICK" || first_command(irc) == "USER" || first_command(irc) == "PASS")
            send_error_message(irc, i, "462", ": You are already registered", irc.client[i].client_sock);
        else
            commands(irc, i);
    }
    return (0);
}

// Funzione per gestire i comandi generali
int handle_command(ft_irc &irc, int i)
{
    i--;
    if (registretion(irc, i) == 1)
        return (1);
    return 0;
}
