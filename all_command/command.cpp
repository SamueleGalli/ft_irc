#include "../header/ft_irc.hpp"

// Funzione per gestire il comando PASS
int process_pass_command(ft_irc &irc, int i)
{
    if (second_command(irc) == irc.pass_server)
    {
        irc.client[i].is_pass = true;
        return (0);
    }
    else if (second_command(irc) == "no")
    {
        std::string message = first_command(irc) + " : Not enough parameters";
        send_error_message("461", message, irc.client[i].client_sock);
    }
    else
        send_error_message("464", ": Password incorrect", irc.client[i].client_sock);
    return (0);
}

int user_command(ft_irc &irc, int i)
{
    int u_client = handle_user(irc, i);
    if (u_client == 0 && irc.client[i].is_pass == true)
    {
        if (irc.client[i].is_nick == true && irc.client[i].is_pass == true && \
        irc.client[i].is_user == true)
        {
            welcome_msg(irc, i);
            irc.client[i].authenticated = true;
        }
        return (0);
    }
    else if (irc.client[i].is_user == true && u_client == 0)
        send_error_message("462", ": You are already registered", irc.client[i].client_sock);
    return (0);
}

void nick_command(ft_irc &irc, int i)
{
    int u_nick = check_nick(second_command(irc), irc, i);
    if (u_nick == 0 && irc.client[i].is_pass == true && \
    (irc.client[i].is_nick == false || irc.client[i].is_nick == true))
    {
        irc.client[i].nick = second_command(irc);
        if (irc.client[i].is_nick == true && irc.client[i].is_pass == true && \
        irc.client[i].is_user == true)
        {
            welcome_msg(irc, i);
            irc.client[i].authenticated = true;
        }
        return ;
    }
}

int registretion(ft_irc &irc, int i)
{
    if (!irc.client[i].authenticated)
    {
        if (first_command(irc) == "PASS" && irc.client[i].is_pass == false)
            return (process_pass_command(irc, i));
        else if (first_command(irc) == "PASS" && irc.client[i].is_pass == true)
        {
            send_error_message("462", ": You are already registered", irc.client[i].client_sock);
            return (0);
        }
        if (first_command(irc) == "NICK")
            nick_command(irc, i);
        else if (first_command(irc) == "USER")
            user_command(irc, i);
    }
    else
    {
        if (first_command(irc) == "NICK" || first_command(irc) == "USER" || first_command(irc) == "PASS")
            send_error_message("462", ": You are already registered", irc.client[i].client_sock);
        std::cout << CYAN <<" Client[" << i << "]: " << RESET;
        colored_message(irc.buffer, CYAN);
        send_error_message("ops", "invalid command", irc.client[i].client_sock);
        //comandi se registrato
        //comando non riconosciuto devi essere registrato
        /*421     ERR_UNKNOWNCOMMAND
                        "<command> :Unknown command"*/
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
