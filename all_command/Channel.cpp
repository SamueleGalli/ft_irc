#include "../header/Channel.hpp"
#include "../header/ft_irc.hpp"  

Channel::Channel(std::string name) 
{
	this->_name = name; 
	this->_num_users = 0;
	this->_max_users = -1;
	this->_topic = "";
    this->have_op = false;
	this->invite_only = false;
	this->has_password = false; 
	this->topic_limited = false;
	this->users_limit = false;
	this->has_key = false;
	
}
Channel::~Channel() {}

Channel::Channel()
{
}

void Channel::addUser(ft_irc &irc, int i)
{
    client_info newClient;
    newClient.user = irc.client[i].user;
    newClient.nick = irc.client[i].nick;
    newClient.realname = irc.client[i].realname;
    newClient.host = irc.client[i].host;
    newClient.server = irc.client[i].server;
    newClient.client_addr = irc.client[i].client_addr;
    newClient.client_sock = irc.client[i].client_sock;
    newClient.client_len = irc.client[i].client_len;
    newClient.quit_received = false;

    users.push_back(newClient);
    _num_users++;
}

void Channel::DeleteUserFromChannel(ft_irc& irc, int i)
{
    std::string nick = irc.client[i].nick;
    if (!operatorUsers.empty())
    {
        for (std::vector<client_info>::iterator it = operatorUsers.begin(); it != operatorUsers.end(); ++it)
        {
            if (it->nick == nick)
            {
                removeOperator(nick);
                break;
            }
        }
    }
    if (!users.empty())
    {
        for (std::vector<client_info>::iterator it = users.begin(); it != users.end(); ++it)
        {
            if (it->nick == nick)
            {
                removeUser(nick);
                break;
            }
        }
    }
}

void Channel::addOperatorUser(const std::string& oper_name, const std::string& nick, int sock)
{
    client_info newOperator;
    newOperator.user = oper_name;
    newOperator.nick = nick;
    newOperator.client_sock = sock;
    newOperator.quit_received = false;
    operatorUsers.push_back(newOperator);
}

void Channel::removeUser(const std::string& nick)
{
    for (std::vector<client_info>::iterator it = users.begin(); it != users.end(); ++it)
    {
        if (it->nick == nick)
        {
            users.erase(it);
            _num_users--;
            return;
        }
    }
}

void Channel::removeOperator(const std::string& oper)
{
    for (std::vector<client_info>::iterator it = operatorUsers.begin(); it != operatorUsers.end(); ++it)
    {
        if (it->nick == oper)
        {
            operatorUsers.erase(it);
            break;
        }
    }
}

void Channel::removeInvited(const std::string& invited)
{
    for (std::vector<client_info>::iterator it = invitedUsers.begin(); it != invitedUsers.end(); ++it)
    {
        if (it->nick == invited)
        {
            invitedUsers.erase(it);
            break;
        }
    }
}

bool Channel::channelHasName(const std::string& name) const 
{
	return _name == name;
}

void    Channel::next_operator(void)
{
    if (operatorUsers.empty())
        addOperatorUser(users[0].user, users[0].nick, users[0].client_sock);
}