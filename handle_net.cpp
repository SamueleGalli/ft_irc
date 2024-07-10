#include "ft_irc.hpp"

void autentication(ft_irc &irc, char *buffer)
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
}

int    handle_client(int client_sock, ft_irc irc)
{
    char buffer[512]; //numero di caratteri messaggio massimi per irc
    ssize_t bytes_received;

    memset(buffer, 0, sizeof(buffer));
    autentication(irc, buffer);
    welcoming_msg(irc);
    while ((bytes_received = recv(client_sock, buffer, 512, 0)) > 0) //ricevo i dati dal client
    {
        buffer[bytes_received] = '\0';
        std::cout << CYAN << "Client: " << buffer << RESET << std::endl;
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
    close(client_sock);
    return (0);
}

int    handle_server(ft_irc irc)
{
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    //prova a creare un socket se fallisce da errore
    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) //SOCK_STREAM = uso indirizzo TCP
    {
        std::cout << RED << "🚨Error: \n(socket creation failed)🚨\n" << RESET;
        return (1);
    }
    //inizializzo la struttura
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET; //AF_INET = indico che usero indirizzi IPv4
    std::stringstream ss(irc.port); //rendo la strina un flusso di dati
    //converto porta
    int port_num;
    ss >> port_num;
    server_addr.sin_port = htons(port_num); //aprendo la porta per la connessione
    //converte l'indirizzo IP da stringa a binario
    if (inet_pton(AF_INET, irc.server.c_str(), &server_addr.sin_addr) <= 0) {
        std::cout << RED << "🚨 Error: (invalid address) 🚨" << RESET << std::endl;
        close(server_sock);
        return 1;
    }
    //assegno indirizzo IP e porta al  socket
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cout << RED << "🚨Error: \n(bind failed)🚨\n" << RESET;
        close(server_sock);
        return (1);
    }
    //abilita il socket a ricevere connessioni con un massimo di dieci in coda
    if (listen(server_sock, 10) < 0)
    {
        std::cout << RED << "🚨Error: \n(listen failed)🚨\n" << RESET;
        close(server_sock);
        return (1);
    }
    std::cout << GREEN << "👂Listening on port " << irc.port << "👂" RESET << std::endl;
    while (1)
    {
        client_len = sizeof(client_addr);
        //se la connessione viene stabilita lavoro dalla parte client
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
        if (client_sock < 0)
        {
            std::cout << RED << "🚨Error: \n(accept failed)🚨\n" << RESET;
            close(server_sock);
            return (1);
        }
        if (handle_client(client_sock, irc) == 1)
            break;
    }
    close(server_sock);
    return (0);
}