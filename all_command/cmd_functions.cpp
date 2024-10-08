#include "../header/ft_irc.hpp"


std::vector<Channel>::iterator findChannel(const std::string& channel_name, std::vector<Channel>& channels) 
{
    for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it) 
    {
        if (it->channelHasName(channel_name)) 
            return it;
    }
    return channels.end();
}	
	
std::vector<client_info>::iterator findUserInChannel(const std::string& nick, std::vector<client_info>& users) 
{
    for (std::vector<client_info>::iterator it = users.begin(); it != users.end(); ++it) 
    {
        if (it->clientHasName(nick))
            return it; 
    }
    return users.end();
}

bool isOperator(const std::string& oper_name, std::vector<client_info>& operatorUsers) 
{
    for (std::vector<client_info>::iterator it = operatorUsers.begin(); it != operatorUsers.end(); ++it) 
    {
        if (it->clientHasName(oper_name)) 
            return true;
    }
    return false;
}

int nick_exist(std::vector<client_info>& clients, const std::string& nickname)
{
	for (std::vector<client_info>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->nick == nickname)
			return 1;
	}
	return 0;
}

int get_user_index(std::vector<client_info>& clients, const std::string& nickname)
{
	int i = 0;
	for (std::vector<client_info>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->nick == nickname)
			return i;
		i++;
	}
	return -1;
}

bool userReceivedInvite(Channel& channel, const std::string& nick)
{
	for (std::vector<client_info>::iterator it = channel.invitedUsers.begin(); it != channel.invitedUsers.end(); ++it)
	{
		if (it->nick == nick)
		 return 1;
	}
	return 0;
}

bool userAlreadyInChannel(Channel& channel, const std::string& nick)
{
	for (std::vector<client_info>::iterator it = channel.users.begin(); it != channel.users.end(); ++it)
	{
		if (it->nick == nick)
		 return 1;
	}
	return 0;
}
