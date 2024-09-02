#include "../../header/ft_irc.hpp"
#include "../../header/Channel.hpp"

/*
ERR_TOOMANYTARGETS
ERR_NOSUCHNICK
ERR_CANNOTSENDTOCHAN*/

void sendMessageToUser(ft_irc& irc, const std::string& sender, const std::string reciver, const std::string& message)
{
    int userIndex = get_user_index(irc.client, sender);
    std::cout << "Debug: User Index = " << userIndex << std::endl;

    if (userIndex != -1 && userIndex < static_cast<int>(irc.client.size()))
    {
        // L'utente esiste, invia il messaggio
        std::string full_message = reciver + " " + message; // `message` già include i caratteri di newline
        client_message(irc, userIndex, "PRIVMSG", full_message);
    }
    else
    {
        // L'utente non esiste, invia un errore se l'indice è valido
        /*if (userIndex != -1)
        {
            std::string errMsg = "401 " + nick + " :No such nick/channel";
            client_message(irc, userIndex, "ERROR", errMsg); // Usa un comando di errore appropriato
        }*/
    }
}

void sendMessageToChannel(ft_irc& irc, const std::string& channelName, const std::string& message, client_info& sender)
{
    // Trova il canale
    std::vector<Channel>::iterator channelIt = findChannel(channelName, irc.channels);
    if (channelIt != irc.channels.end())
    {
        // Il canale esiste
        std::string privmsg = ":" + sender.nick + " PRIVMSG " + channelName + message + "\r\n";
        // Debug: stampa gli utenti del canale
        std::cout << "Debug: Channel " << channelName << " found. Users count: " << channelIt->users.size() << std::endl;
        
        // Invia il messaggio a tutti gli utenti del canale
        for (size_t i = 0; i < channelIt->users.size(); i++)
        {
            client_info user = channelIt->users[i];
            std::cout << "User Nick: " << user.nick << " Socket FD: " << user.client_sock << std::endl;
            // Non inviare il messaggio al mittente
            std::cout << "Sending message to user " << user.nick << " on socket " << user.client_sock << std::endl;
            send(user.client_sock, privmsg.c_str(), privmsg.size(), 0);
        }
    }
    /*else
    {
        // Il canale non esiste, invia un errore al mittente
        std::string errMsg = "403 " + channelName + " :No such channel\r\n";
        send(sender.client_sock, errMsg.c_str(), errMsg.size(), 0);
    }*/
}

bool    valid_message(const std::string& target)
{
    std::cout << "target: \'" << target << "\'" <<  std::endl;
    size_t i = 0;
    for (;target[i] == ' '; i++)
    {        
    }
    if (target[i] != ' ' && target[i] != 0)
        return true;
    return false;
}

// Funzione per gestire il comando PRIVMSG
void privmsg_command(ft_irc& irc, int i, const std::string& target)
{
    // Separare il nome del canale o del destinatario dal messaggio
    size_t pos = target.find(' ');
    // ERR_NORECIPIENT (411): Nessun destinatario specificato
    if (target.empty() || target[0] == ':')
    {
        std::string errMsg = "411 " + irc.client[i].nick + " :No recipient given " + first_command(irc) + "\r\n";
        send(irc.client[i].client_sock, errMsg.c_str(), errMsg.size(), 0);
        return;
    }
    
    std::string channel_name = target.substr(0, pos);
    std::string msg = target.substr(pos + 1);
    std::cout << msg << std::endl;

    // ERR_NOTEXTTOSEND (412): Nessun testo da inviare
    if (valid_message(extract_message(target)) == false)
    {
        std::string errMsg = "412 " + irc.client[i].nick + " :No text to send\r\n";
        send(irc.client[i].client_sock, errMsg.c_str(), errMsg.size(), 0);
        return;
    }

    // Se il destinatario è un canale
    if (channel_name[0] == '#')
    { 
        std::vector<Channel>::iterator it = findChannel(channel_name, irc.channels);

        // ERR_NOSUCHNICK (401): Il canale non esiste
        if (it == irc.channels.end())
        {
            std::string errMsg = "403 " + channel_name + " :No such channel\r\n";
            send(irc.client[i].client_sock, errMsg.c_str(), errMsg.size(), 0);
            return;
        }

        /*// ERR_CANNOTSENDTOCHAN (404): L'utente non può inviare messaggi al canale (es. bannato)
        if (!it->canSendMessage(irc.client[i]))
        { 
            std::string errMsg = "404 " + channel_name + " :Cannot send to channel\r\n";
            send(irc.client[i].client_sock, errMsg.c_str(), errMsg.size(), 0);
            return;
        }*/

        // Invia il messaggio a tutti i membri del canale
        sendMessageToChannel(irc, channel_name, msg, irc.client[i]);
    }
    else
    { // Se il destinatario è un utente
        int userIndex = get_user_index(irc.client, channel_name);

        // ERR_NOSUCHNICK (401): L'utente specificato non esiste
        if (userIndex == -1)
        {
            std::string errMsg = "401 " + channel_name + " :No such nick/channel\r\n";
            send(irc.client[i].client_sock, errMsg.c_str(), errMsg.size(), 0);
            return;
        }

        // Invia il messaggio all'utente
        sendMessageToUser(irc, channel_name, irc.client[i].nick, msg);
    }
}

// ERR_TOOMANYTARGETS (407): Gestione dell'invio a troppi destinatari (in caso di lista separata da virgole)
/*std::vector<std::string> recipients = splitTargets(channel_name); // Presupponendo che splitTargets separi i destinatari con virgole
if (recipients.size() > MAX_RECIPIENTS)
{ // Definisci MAX_RECIPIENTS come appropriato per il tuo protocollo
    std::string errMsg = "407 " + irc.client[i].nick + " :Too many targets\r\n";
    send(irc.client[i].client_sock, errMsg.c_str(), errMsg.size(), 0);
    return;
}
*/