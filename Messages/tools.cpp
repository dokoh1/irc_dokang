/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sihkang <sihkang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:16:22 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/20 20:31:30 by sihkang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "tools.hpp"
#include "Response.hpp"

bool isCorrectPassword(serverInfo &info, std::string &client_pw)
{
	return (info.server_pwd == client_pw) ? true : false;
}

bool isCommand(IRCMessage msg, std::string cmd)
{
	if (msg.command.find(cmd) != std::string::npos)
		return (true);
	return (false);
}

std::string getMessageParams(IRCMessage message)
{
	std::string params = message.params[0];

	for (int i = 1; i < message.numParams; i++)
	{
		params += " " + message.params[i];
	}
	return (params);
}

std::string aftercolonConcat(IRCMessage message)
{
	std::string concatString = "";
	int i;
	for (i = 0; i < message.numParams; i++)
	{
		if (message.params[i].front() == ':')
			break;
	}
	if (i == message.numParams)
		return concatString;
	concatString += message.params[i++].erase(0, 1);
	for (;i < message.numParams; i++)
	{
		concatString += (' ' + message.params[i]);
	}
	return concatString;
}

User& findUser(serverInfo &info, std::string nick)
{
	std::list<User>::iterator it;
	
	for (it = info.usersInServer.begin(); it != info.usersInServer.end(); ++it)
	{
		if ((*it).nick == nick)
			return (*it);
	}
	return (*info.usersInServer.begin());
}

User& findUser(serverInfo &info, int client_fd)
{
	std::list<User>::iterator it;
	
	for (it = info.usersInServer.begin(); it != info.usersInServer.end(); ++it)
	{
		if ((*it).client_fd == client_fd)
			return (*it);
	}
	return (*info.usersInServer.begin());
}

User& findUser(Channel &ch, int client_fd)
{
	std::list<User>::iterator it;
	
	for (it = ch.channelUser.begin(); it != ch.channelUser.end(); ++it)
	{
		if ((*it).client_fd == client_fd)
			return (*it);
	}
	return (*ch.channelUser.begin());
}

User& findUser(Channel &ch, std::string nick)
{
	std::list<User>::iterator it;
	
	for (it = ch.channelUser.begin(); it != ch.channelUser.end(); ++it)
	{
		if ((*it).nick == nick)
			return (*it);
	}
	return (*ch.channelUser.begin());
}


User& findOPUser(Channel &ch, int client_fd)
{
	std::list<User>::iterator it;
	
	for (it = ch.operator_user.begin(); it != ch.operator_user.end(); ++it)
	{
		if ((*it).client_fd == client_fd)
			return (*it);
	}
	return (*ch.operator_user.begin());
}

Channel& findChannel(serverInfo &info, std::string chName)
{
	std::list<Channel>::iterator it;
	
	for (it = ++(info.channelInServer.begin()); it != info.channelInServer.end(); ++it)
	{
		if ((*it).name == chName)
			return (*it);
	}
	return (*info.channelInServer.begin());
}

std::string channelUserList(Channel &requestedChannel)
{
	std::string userList = "";
	std::list<User>::iterator it;
	for (it = ++(requestedChannel.channelUser.begin()); it != requestedChannel.channelUser.end();)
	{
		if (findOPUser(requestedChannel, (*it).client_fd).nick != "")
		{
			userList += "@";
		}
		userList += (*it).nick;
		if (++it != requestedChannel.channelUser.end())
			userList += " ";
	}
	std::cout << "userList: "<< userList << '\n';
	return (userList);
}

void setChannelMode(Channel &ch, bool i, bool t, bool k, bool o, bool l)
{
	ch.opt[MODE_i] = i;
	ch.opt[MODE_t] = t;
	ch.opt[MODE_k] = k;
	ch.opt[MODE_o] = o;
	ch.opt[MODE_l] = l;
}

std::string getChannelMode(Channel &ch)
{
	std::string setting = "+";
	if (ch.opt[MODE_i] == true)
		setting += "i";
	if (ch.opt[MODE_t] == true)
		setting += "t";
	if (ch.opt[MODE_k] == true)
		setting += "k";
	if (ch.opt[MODE_o] == true)
		setting += "o";
	if (ch.opt[MODE_l] == true)
		setting += "l";
	return (setting);
}

void changeChannelMode(int client_fd, Channel &ch, IRCMessage msg)
{
	if (msg.params[1][0] == '+')
	{
		modifyChannelOpt(ch, msg);
	}
	else if (msg.params[1][0] == '-')
	{
		unsetChannelOpt(ch, msg);
	}
	else
	{
		Response::send_message(client_fd, "dokang 401 " + (findUser(ch, client_fd)).nick 
							+ msg.params[1][0] + " :No such nick");
	}
}

void modifyChannelOpt(Channel &ch, IRCMessage msg)
{
	std::string setting = msg.params[1];
	int arguIdx = 2;
	
	if (setting.find('i') != std::string::npos)
		ch.opt[MODE_i] = true;
	if (setting.find('t') != std::string::npos)
		ch.opt[MODE_t] = true;
	if (setting.find('k') != std::string::npos)
	{
		ch.key = msg.params[arguIdx++];
		ch.opt[MODE_k] = true;
	}
	if (setting.find('o') != std::string::npos)
	{
		ch.operator_user.push_back(findUser(ch, msg.params[arguIdx++]));
		ch.opt[MODE_o] = true;
	}
	if (setting.find('l') != std::string::npos)
	{
		ch.user_limit = atoi(msg.params[arguIdx++].c_str());
		if (ch.user_limit > 0 && ch.user_limit < 100)
		ch.opt[MODE_l] = true;
	}
}

void unsetChannelOpt(Channel &ch, IRCMessage msg)
{
	std::string setting = msg.params[1];
	
	if (setting.find('i') != std::string::npos)
		ch.opt[MODE_i] = false;
	if (setting.find('t') != std::string::npos)
		ch.opt[MODE_t] = false;
	if (setting.find('k') != std::string::npos)
	{
		ch.opt[MODE_k] = false;
		ch.key = "";
	}
	if (setting.find('o') != std::string::npos)
	{
		User user = findUser(ch, msg.params[2]);
		for (std::list<User>::iterator it = ch.operator_user.begin(); it != ch.operator_user.end(); ++it)
		{
			if ((*it).nick == user.nick)
			{
				ch.operator_user.erase(it);
				break ;
			}
		}
		ch.opt[MODE_o] = false;
	}
	if (setting.find('l') != std::string::npos)
	{
		ch.opt[MODE_l] = false;
		ch.user_limit = 9999;
	}
}
std::string getCreatedTimeUnix()
{
	std::stringstream ss;
	std::string ret;
	
	std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    std::time_t unix_timestamp = std::chrono::system_clock::to_time_t(now);
	ss << unix_timestamp;
	ss >> ret;
    return (ret);
}

std::string getCreatedTimeReadable()
{
	std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    std::time_t unix_timestamp = std::chrono::system_clock::to_time_t(now);
    std::tm* ptm = std::localtime(&unix_timestamp);
    char buffer[32];
    // 포맷팅된 시간 문자열 생성
    std::strftime(buffer, 32, "%H:%M:%S %b %d %Y", ptm);
	std::string ret = buffer;
	return ret;
}