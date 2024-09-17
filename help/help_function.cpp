#include "../header/ft_irc.hpp"

std::string extract_message(const std::string &buffer)
{
    size_t pos = buffer.find(':');
    if (pos != std::string::npos) {
        return buffer.substr(pos + 1);
    }
    return "";
}

void init_poll(ft_irc &irc, int &sock)
{
    struct pollfd pfd; //creo struct pollfd
    pfd.fd = sock; //associo il socket a fd cosi che pfd lo monitori
    pfd.events = POLLIN; //permetto l'ascolto degli eventi
    pfd.revents = 0; //definisce che pfd dovra monitorare i dati inviati
    irc.p_fds.push_back(pfd); //inserisco pfd nel vettore
}

void removeChars(std::string& str, const char charsToRemove)
{
    str.erase(std::remove(str.begin(), str.end(), charsToRemove), str.end());
}

bool enough_elements(const std::string &input)
{
    std::istringstream iss(input);
    std::string word;
    int count = 0;

    while (iss >> word)
    {
        if (count == 0 && (word != "NICK" && word != "USER"))
            return (false);
        count++;
    }
    return (count == 2);
}

bool    check_info(ft_irc irc, int i)
{
    return (enough_elements(irc.client[i].nick) && enough_elements(irc.client[i].user));
}

std::string trim(const std::string& str)
{
    // Trova la prima posizione di un carattere non spazio
    size_t start = str.find_first_not_of(' ');
    // Trova l'ultima posizione di un carattere non spazio
    size_t end = str.find_last_not_of(' ');

    // Se la stringa è tutta spazi o vuota, ritorna una stringa vuota
    if (start == std::string::npos) {
        return "";
    }

    // Ritorna la sottostringa tra 'start' e 'end'
    return str.substr(start, end - start + 1);
}

void update_user_list(ft_irc& irc, int i, std::vector<Channel>::iterator it)
{
	std::string message;
	std::string users_list;
	
	message =" = " + it->_name + " :";
	for (std::vector<client_info>::iterator op_it = it->operatorUsers.begin(); op_it != it->operatorUsers.end(); ++op_it)
		users_list += "@" + op_it->nick + " ";
	std::vector<client_info>::iterator op_it = it->operatorUsers.begin();
	bool is_operator = false;
	for (std::vector<client_info>::iterator user_it = it->users.begin(); user_it != it->users.end(); ++user_it)
	{
		for (;op_it != it->operatorUsers.end(); ++op_it)
		{
			if (user_it->nick == op_it->nick)
			{
				is_operator = true;
				break;
			}
		}
		if (is_operator == true)
		{
			is_operator = false;
			continue;
		}
		users_list += user_it->nick + " ";
	}

	message += users_list;
	unsigned long int t;
	
	for (t = 0; t < it->users.size(); t++)
	{
		std::cout << it->users[t].nick << std::endl;
		if (it->isMember(irc.client[t]) == true || it->isMemberOperator(irc.client[t]) == true)
		{
			std::cout << "SENDING " << message << std::endl;
			send_error_message(irc, i, "353", message, irc.client[t].client_sock);
		}
	}
	message =it->_name + " :End of /NAMES list";
	for (t = 0; t < it->users.size(); t++)
	{
		if (it->isMember(irc.client[t]) == true || it->isMemberOperator(irc.client[t]) == true)
			send_error_message(irc, i, "366", message, irc.client[t].client_sock);
	}
}

void quitting_channels(ft_irc& irc, int i, unsigned int t, std::string& message)
{
    for (t = 0; t < irc.client.size(); t++)
    {
        for (std::vector<Channel>::iterator ch_it = irc.channels.begin(); ch_it != irc.channels.end(); ++ch_it)
        {
            if (findUserInChannel(irc.client[i].nick, ch_it->users) != ch_it->users.end() && 
                findUserInChannel(irc.client[t].nick, ch_it->users) != ch_it->users.end())
            {
                message = irc.client[i].nick + " has quit (Read error : Connection reset by peer)";
                client_message_all_users(irc, i, (int)t, "", message);
            }
        }
        std::vector<Channel>::iterator it = irc.channels.begin();
        while (it != irc.channels.end())
        {
            it->DeleteUserFromChannel(irc, i);
            if (it->_num_users <= 0 || it->operatorCount() <= 0)
                it = irc.channels.erase(it);
            else
            {
                ++it;
                update_user_list(irc, i, it);
            }
        }
    }
    close(irc.client[i].client_sock);

    for (std::vector<struct pollfd>::iterator it = irc.p_fds.begin(); it != irc.p_fds.end(); ++it)
    {
        if (it->fd == irc.client[i].client_sock)
        {
            it = irc.p_fds.erase(it);
            break;
        }
    }
    irc.client.erase(irc.client.begin() + i);
}

bool Channel::isMember(const client_info& user)
{
    return std::find(users.begin(), users.end(), user) != users.end();
}

bool Channel::isMemberOperator(const client_info& user_operator)
{
    return std::find(operatorUsers.begin(), operatorUsers.end(), user_operator) != operatorUsers.end();
}