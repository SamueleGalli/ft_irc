#include "../../header/ft_irc.hpp"

void	user(ft_irc irc, int i, std::string message, std::vector<Channel>::iterator &it, std::string comment)
{
	for (size_t t = 0; t < it->operatorUsers.size(); t++)
	{
		if (it->operatorUsers[t].quit_received == false)
		{
			it->operatorUsers[t].quit_received = true;
			if (comment.empty())
				message = ":Client Quit";
			else
				message = ":" + comment;
			if (it->operatorUsers[t].nick != irc.client[i].nick)
				client_message_user(irc, i, "QUIT", message, it->operatorUsers[t]);
		}
	}
	if (!it->users.empty())
	{
		for (size_t t = 0; t < it->users.size(); t++)
		{
			if (it->users[t].quit_received == false)
			{
				it->users[t].quit_received = true;
				if (comment.empty())
					message = ":Client Quit";
				else
					message = ":" + comment;
				if (it->users[t].nick != irc.client[i].nick)
					client_message_operator(irc, i, "QUIT", message, it->users[t]);
			}
		}
	}
}

void	listing(ft_irc irc, std::vector<Channel>::iterator &it)
{
	std::string message = user_list(*it);
	std::string end_users = it->_name + " :End of /NAMES list";
	if (it->operatorUsers.size() == 1 && it->num_user() == 1)
	{
		std::cout << "crasha\n";
		if (!it->operatorUsers.empty())
		{
			for (unsigned int op = 0;op != it->operatorUsers.size(); op++)
			{
				if (findUserInChannel(it->operatorUsers[op].nick, it->operatorUsers) != it->operatorUsers.end())
				{
					send_error_message(irc, op, "353", message, it->operatorUsers[op].client_sock);
					send_error_message(irc, op, "366", end_users, it->operatorUsers[op].client_sock);
				}
			}
		}
	}
	else
		update_channel_list(irc, *it);
}

void	quit_single(ft_irc irc, int i)
{
	std::vector<Channel>::iterator it = irc.channels.begin();
	while (it != irc.channels.end())
	{
		if (irc.channels.empty())
			continue;
		it->DeleteUserFromChannel(irc, i);
		if (it->_num_users == 0)
			it = irc.channels.erase(it);
		it++;
	}
}

void quit_command(ft_irc &irc, int i, const std::string& comment)
{
    std::string message;
	if (comment == "Konversation terminated!")
		quit_single(irc, i);
	else
	{
		if (!irc.channels.empty())
		{
			std::vector<Channel>::iterator it = irc.channels.begin();
			while (it != irc.channels.end())
			{
				it->DeleteUserFromChannel(irc, i);
				if (it->_num_users == 0)
					it = irc.channels.erase(it);
				else if (it->operatorCount() == 0)
				{
					it->next_operator();
					message = it->_name + " +o " + it->operatorUsers[0].nick;
					for (size_t t = 0; t < it->users.size(); t++)
						client_message_in_channel(irc, *it, i, (int)t, "MODE", message);
					user(irc, i, message, it, comment);
					listing(irc, it);
				}
				else
				{
					user(irc, i, message, it, comment);
					listing(irc, it);
					++it;
				}
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
