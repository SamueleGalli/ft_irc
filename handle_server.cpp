#include "ft_irc.hpp"

int    handle_server(ft_irc irc)
{
    //prova a creare un socket se fallisce da errore
    if ((irc.server.server_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) //SOCK_STREAM = uso indirizzo TCP
    {
        print_message("🚨Error: \n(socket creation failed)🚨", RED , irc);
        return (1);
    }
    //inizializzo la struttura
    memset(&irc.server.server_addr, 0, sizeof(irc.server.server_addr));
    irc.server.server_addr.sin_family = AF_INET; //AF_INET = indico che usero indirizzi IPv4
    std::stringstream ss(irc.port); //rendo la strina un flusso di dati
    //converto porta
    int port_num;
    ss >> port_num;
    irc.server.server_addr.sin_port = htons(port_num); //aprendo la porta per la connessione
    //converte l'indirizzo IP da stringa a binario
    if (inet_pton(AF_INET, "127.0.0.1", &irc.server.server_addr.sin_addr) <= 0)
    {
        print_message("🚨Error: \n(invalid address)🚨", RED , irc);
        close(irc.server.server_sock);
        return 1;
    }
    //assegno indirizzo IP e porta al  socket
    if (bind(irc.server.server_sock, (struct sockaddr *)&irc.server.server_addr, sizeof(irc.server.server_addr)) < 0)
    {
        print_message("🚨Error: \n(bind failed)🚨", RED , irc);
        close(irc.server.server_sock);
        return (1);
    }
    //abilita il socket a ricevere connessioni con un massimo di dieci in coda
    if (listen(irc.server.server_sock, 10) < 0)
    {
        print_message("🚨Error: \n(listen failed)🚨", RED , irc);
        close(irc.server.server_sock);
        return (1);
    }

    print_message("Server: listening on port " + irc.port, YELLOW, irc);
    while (1)
    {
        irc.client.client_len = sizeof(irc.client.client_addr);
        //se la connessione viene stabilita lavoro dalla parte client
        irc.client.client_sock = accept(irc.server.server_sock, (struct sockaddr *)&irc.client.client_addr, &irc.client.client_len);
        if (irc.client.client_sock < 0)
        {
            print_message("🚨Error: \n(accept failed)🚨", RED , irc);
            close(irc.server.server_sock);
            return (1);
        }
        if (handle_client(irc) == 1)
            break;
    }
    close(irc.server.server_sock);
    return (0);
}