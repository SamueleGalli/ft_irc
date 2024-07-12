#include "ft_irc.hpp"

/*void autentication(ft_irc &irc, char *buffer)
{
    std::cout << YELLOW << "Server: you need to authenticate first" << RESET << std::endl;
    while (1)
    {
        std::istringstream iss(buffer);
        std::string command;
        iss >> command;
        std::cout << MAGENTA << "INSERT NICKNAME (NICK + nickname)\n" << RESET;
        std::getline(std::cin, irc.nick);
        std::cout << MAGENTA << "INSERT USERNAME (USER + username)\n" << RESET;
        std::getline(std::cin, irc.user);
        std::cout << MAGENTA << "INSERT PASSWORD (PASS + password)\n" << RESET;
        std::getline(std::cin, irc.pass);
        if (check_info(irc) == true && !irc.nick.empty() && !irc.user.empty() && !irc.pass.empty())
        {
            std::cout << GREEN << "authentication successful" << RESET << std::endl;
            break;
        }
        else
        {
            std::cout << RED << "authentication failed" << RESET << std::endl;
            std::cout << YELLOW << "missing information" << RESET << std::endl;
        } 
    }
}*/

int    handle_client(ft_irc irc)
{
    //ssize_t bytes_received;

    print_message("👋Welcome to the server!👋", GREEN);
    print_message("👋Please provide your nickname, username and password.👋", GREEN);
    
    memset(irc.buffer, 0, sizeof(irc.buffer));
    /*authentication(irc);
    welcoming_msg(irc);
    while ((bytes_received = recv(irc.client.client_sock, irc.buffer, 512, 0)) > 0) //ricevo i dati dal client
    {
        irc.buffer[bytes_received] = '\0';
        std::cout << CYAN << "Client: " << irc.buffer << RESET << std::endl;
    }
    //se bytes_received riceve 0 significa che il client ha chiuso la connessione
    //altrimenti ci sono stati errori
    if (bytes_received == 0)
    {
        std::cout << RED << "Client disconnected (GOODBYE👋👋)" << RESET << std::endl;
        return (1);
    }
    else
    {
        std::cout << RED << "🚨Error: " << strerror(errno) << "🚨" << RESET << std::endl;
    }
    //chiusura socket
    close(irc.client.client_sock);
    */return (0);
}