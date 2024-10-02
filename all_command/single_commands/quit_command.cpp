#include "../../header/ft_irc.hpp"


int	member_quit(std::string nick ,std::vector<Channel>::iterator& it)
{
	for (size_t t = 0; t < it->users.size();)
	{
		if (it->users[t].nick == nick)
			return (1);
		t++;
	}
	return (0);
}

void quit_konversation(ft_irc& irc, int i, std::vector<Channel>::iterator& it, std::string& quit_message, std::string nick)
{
	std::string message;
	for (size_t t = 0; t < it->users.size(); t++)
	{
		if (irc.client[i].nick != it->users[t].nick && member_quit(nick, it) == 1)
		{
			it->DeleteUserFromChannel(irc, i);
			if (it->operatorCount() == 0 && it->_num_users > 0)
			{
				it->next_operator();
				message = it->_name + " +o " + it->operatorUsers[0].nick;
				for (size_t t = 0; t < it->users.size(); t++)
					client_message_in_channel(irc, *it, i, (int)t, "MODE", message);
			}
			client_message_in_channel(irc, *it, i, (int)t, "QUIT", quit_message);
			reply_to_channel(irc, t, *it);
		}
	}
	if (it->_num_users == 0)
		it = irc.channels.erase(it);
	else
		it++;
}

void remove_client_from_channels(ft_irc& irc, int i, std::string& quit_message)
{	
	std::string nick = irc.client[i].nick;
	for (std::vector<Channel>::iterator it = irc.channels.begin(); it != irc.channels.end();)
		quit_konversation(irc, i, it, quit_message, nick);
}


void quit_command(ft_irc &irc, int i, const std::string& comment)
{
	std::string message;
	if (!comment.empty())
		message =":" + comment;
	else
		message = ":Client Quit";
	remove_client_from_channels(irc, i, message);
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