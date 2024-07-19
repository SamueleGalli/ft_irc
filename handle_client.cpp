#include "ft_irc.hpp"

void authentication(ft_irc &irc)
{
    std::cout << YELLOW << "Server: you need to authenticate first" << RESET << std::endl;
    while (1)
    {
        std::istringstream iss(irc.buffer);
        std::string command;
        iss >> command;
        std::cout << MAGENTA << "INSERT NICKNAME (NICK + nickname)\n" << RESET;
        std::getline(std::cin, irc.client.nick);
        std::cout << MAGENTA << "INSERT USERNAME (USER + username)\n" << RESET;
        std::getline(std::cin, irc.client.user);
        std::cout << MAGENTA << "INSERT PASSWORD (PASS + password)\n" << RESET;
        std::getline(std::cin, irc.client.pass);
        if (check_info(irc) == true && !irc.client.nick.empty() && !irc.client.user.empty() && !irc.client.pass.empty())
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
}

int checking_pass_client_server(ft_irc &irc)
{
    //struct pollfd formed by a file descriptor and its events
    struct pollfd fds[1];
    print_message("client connected please insert password", GREEN);
    fds[0].fd = irc.client.client_sock; //file descriptor for poll to client socket
    fds[0].events = POLLIN; //poll waits for incoming data
    while (1)
    {
        int ret = poll(fds, 1, 60000); //poll wati for about 60 seconds
        //ret == -1 means error
        //ret == 0 means timeout
        //ret > 0 means success event finded
        if (ret == -1)
        {
            // Error occurred
            print_message("Error: poll failed.", RED);
            close(irc.client.client_sock);
            return (1);
        }
        else if (ret == 0)
        {
            // Timeout occurred
            print_message("Error: Timeout occurred.", RED);
            close(irc.client.client_sock);
            return (1);
        }
        // Check if there is data available
        if (fds[0].revents & POLLIN)
        {
            // Read data from the client socket
            memset(irc.buffer, 0, sizeof(irc.buffer));
            ssize_t bytes_received = recv(irc.client.client_sock, irc.buffer, sizeof(irc.buffer) - 1, 0);

            //if true error
            if (bytes_received <= 0)
            {
                if (bytes_received == 0) //Client disconnected
                    print_message("Client disconnected.", RED);
                else    //problem with recived data
                    print_message("Error: recv failed.", RED);
                close(irc.client.client_sock);
                return (1);
            }
            irc.buffer[bytes_received - 1] = '\0';
            std::string client_password(irc.buffer);
            // Check if the password matches
            if (client_password == irc.pass_server)
            {
                print_message("👋Welcome to the server!👋",GREEN);
                print_message("👋Please provide your nickname, username and password.👋",GREEN);
                memset(irc.buffer, 0, sizeof(irc.buffer));
                authentication(irc);
                welcoming_msg(irc);
                break;
            }
            else
                print_message("Error: Incorrect password. Please try again.", RED);
       }
    }
    return (0);
}

int    handle_client(ft_irc irc)
{
    if (checking_pass_client_server(irc) == 1)
        return (1);
    return (0);
}