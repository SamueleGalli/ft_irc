#include "ft_irc.hpp"

//il server aspetta USER E NICK se non completi l'autenticazione
//gestione errori nick user (nick gia in uso,non abbastanza parametri ect..)
void authentication(ft_irc &irc)
{
    print_message("Server: you need to authenticate first", YELLOW, irc);
    while (1)
    {
        print_message("INSERT NICK (NICK + nickname)", RED, irc);
        if (receive_data(irc) > 0)
            irc.client.nick = irc.buffer;
        print_message("INSERT USERNAME (USER + username)", RED, irc);
        if (receive_data(irc) > 0)
            irc.client.user = irc.buffer;
        if (check_info(irc) == true && !irc.client.nick.empty() && !irc.client.user.empty())
        {
            print_message("Authentication successful", RED, irc);
            break;
        }
        else
        {
            print_message("Authentication failed", RED , irc);
            print_message("Missing information", YELLOW, irc);
        } 
    }
}

int pass_checking(ft_irc &irc)
{
    //Initialize buffer and read data from the client socket
    if (receive_data(irc) <= 0)
    {
        print_message("Client disconnected or error receiving data.", RED , irc);
        close(irc.client.client_sock);
        return (1);
    }
    std::string client_password(irc.buffer);
    // Check if the password matches
    if (client_password == irc.pass_server)
    {
        print_message("👋Welcome to the server!👋",RED, irc);
        return (0);
    }
    else
    {
        print_message("Error: Incorrect password. Exiting...", RED , irc);
        return (1);
    }
}

int checking_pass_client_server(ft_irc &irc)
{
    //struct pollfd formed by a file descriptor and its events
    irc.p_fds[0].fd = irc.client.client_sock; //file descriptor for poll to client socket
    irc.p_fds[0].events = POLLIN; //poll waits for incoming data
    print_message("client connected please insert password", RED, irc);
    while (1)
    {
        int ret = poll(irc.p_fds, 1, 60000); //poll wait for about 60 seconds
        //ret == -1 means error
        //ret == 0 means timeout
        //ret > 0 means success event finded
        if (ret == -1) // Error occurred
        {
            print_message("Error: poll failed.", RED , irc);
            close(irc.client.client_sock);
            return (1);
        }
        else if (ret == 0) // Timeout occurred
        {
            print_message("Error: Timeout occurred.", RED , irc);
            close(irc.client.client_sock);
            return (1);
        }
        // Check if there is data available
        if (irc.p_fds[0].revents & POLLIN)
        {
            if (pass_checking(irc) == 0)
                return (0);
            else
                return (1);
        }
    }
    return (0);
}

int    handle_client(ft_irc irc)
{
    if (checking_pass_client_server(irc) == 1)
        return (1);
    print_message("👋Please provide your nickname, username and password.👋",RED, irc);
    memset(irc.buffer, 0, sizeof(irc.buffer));
    authentication(irc);
    welcoming_msg(irc);
    return (0);
}