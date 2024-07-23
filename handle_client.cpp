#include "ft_irc.hpp"

// Funzione per leggere i dati in arrivo dal socket e processarli
int process_incoming_data(ft_irc &irc)
{
    int bytes = recv(irc.client.client_sock, irc.buffer, sizeof(irc.buffer) - 1, 0);
    if (bytes == 0)
    {
        colored_message("🚨Error: \n(connection closed)🚨", RED);
        return 1;
    }
    else if (bytes < 0)
    {
        perror("recv");
        return 1;
    }
    irc.buffer[bytes] = '\0';
    if (first_command(irc) == "CAP" && second_command(irc) == "LS 302")
        return 0;
    if (handle_command(irc) == 1)
        return 1;
    return 0;
}

// Funzione per eseguire il poll e gestire gli eventi in arrivo
int poll_and_handle(ft_irc &irc)
{
    int result = poll(irc.p_fds, 1, -1);
    if (result < 0)
    {
        colored_message("🚨Error: \n(poll failed)🚨", RED);
        return 1;
    }
    if (irc.p_fds[0].revents & POLLIN)
    {
        if (process_incoming_data(irc) == 1)
            return 1;
    }
    return 0;
}

// Funzione principale per gestire il client
int handle_client(ft_irc &irc)
{
    init_poll(irc);
    while (1)
    {
        if (poll_and_handle(irc) == 1)
            return 1;
    }
    return 0;
}
