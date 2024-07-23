#include "ft_irc.hpp"

// Funzione per la creazione del socket
int create_socket(int &sockfd)
{
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        colored_message("🚨Error: \n(socket creation failed)🚨", RED);
        perror("socket");
        return 1;
    }
    return 0;
}

// Funzione per impostare SO_REUSEADDR
int set_reuse_address(int sockfd)
{
    int reuse = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
        perror("setsockopt");
        close(sockfd);
        return 1;
    }
    return 0;
}

// Funzione per inizializzare la struttura server_addr
void initialize_address(struct sockaddr_in &server_addr, const std::string &port)
{
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;

    std::stringstream ss(port);
    int port_num;
    ss >> port_num;
    server_addr.sin_port = htons(port_num);
}

// Funzione per il binding del socket
int bind_socket(int sockfd, const struct sockaddr_in &server_addr)
{
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        colored_message("🚨Error: \n(bind failed)🚨", RED);
        perror("bind");
        close(sockfd);
        return 1;
    }
    return 0;
}

// Funzione per abilitare il socket a ricevere connessioni
int start_listening(int sockfd)
{
    if (listen(sockfd, 10) < 0)
    {
        colored_message("🚨Error: \n(listen failed)🚨", RED);
        perror("listen");
        close(sockfd);
        return 1;
    }
    return 0;
}

// Funzione per accettare e gestire connessioni
int accept_connections(ft_irc &irc)
{
    irc.client.client_len = sizeof(irc.client.client_addr);
    irc.client.client_sock = accept(irc.server.server_sock, (struct sockaddr *)&irc.client.client_addr, &irc.client.client_len);
    if (irc.client.client_sock < 0)
    {
        colored_message("🚨Error: \n(accept failed)🚨", RED);
        perror("accept");
        close(irc.server.server_sock);
        return 1;
    }
    if (handle_client(irc) == 1)
        return 1;
    return 0;
}

// Funzione principale per gestire il server
int handle_server(ft_irc &irc)
{
    // Creazione del socket
    if (create_socket(irc.server.server_sock) == 1)
        return 1;

    // Impostare SO_REUSEADDR
    if (set_reuse_address(irc.server.server_sock) == 1)
        return 1;

    // Inizializzazione della struttura server_addr
    initialize_address(irc.server.server_addr, irc.port);

    // Bind
    if (bind_socket(irc.server.server_sock, irc.server.server_addr) == 1)
        return 1;

    // Abilita il socket a ricevere connessioni
    if (start_listening(irc.server.server_sock) == 1)
        return 1;

    colored_message("Server: listening on port " + irc.port, YELLOW);

    // Accetta e gestisce connessioni
    while (1)
    {
        if (accept_connections(irc) == 1)
            break;
    }

    close(irc.server.server_sock);
    return 0;
}
