/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sihkang <sihkang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:16:22 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/14 16:55:25 by sihkang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "tools.hpp"

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

User *findUser(serverInfo &info, std::string nick)
{
	std::list<User *>::iterator it;
	
	for (it = info.usersInServer.begin(); it != info.usersInServer.end(); ++it)
	{
		if ((*it)->nick == nick)
			return (*it);
	}
	return (*it);
}

User *findUser(serverInfo &info, int client_fd)
{
	std::list<User *>::iterator it;
	
	for (it = info.usersInServer.begin(); it != info.usersInServer.end(); ++it)
	{
		if ((*it)->client_fd == client_fd)
			return (*it);
	}
	return (*it);
}

User *findUser(Channel *ch, int client_fd)
{
	std::list<User *>::iterator it;
	
	for (it = ch->channelUser.begin(); it != ch->channelUser.end(); ++it)
	{
		if ((*it)->client_fd == client_fd)
			return (*it);
	}
	return (*it);
}

Channel *findChannel(serverInfo &info, std::string chName)
{
	std::list<Channel *>::iterator it;
	
	for (it = info.channelInServer.begin(); it != info.channelInServer.end(); ++it)
	{
		if ((*it)->name == chName)
			return (*it);
	}
	return (*it);
}

std::string channelUserList(Channel *requestedChannel)
{
	std::string userList = "";
	std::list<User *>::iterator it;
	for (it = requestedChannel->channelUser.begin(); it != requestedChannel->channelUser.end();)
	{
		if ((*it)->nick == requestedChannel->operator_user->nick)
			userList += "@";
		userList += (*it)->nick;
		if (++it != requestedChannel->channelUser.end())
			userList += " ";
	}
	std::cout << "userList: "<< userList << '\n';
	return (userList);
}

void setChannelMode(Channel *ch, bool i, bool t, bool k, bool o, bool l)
{
	ch->opt[MODE_i] = i;
	ch->opt[MODE_t] = t;
	ch->opt[MODE_k] = k;
	ch->opt[MODE_o] = o;
	ch->opt[MODE_l] = l;
}

std::string getChannelMode(Channel *ch)
{
	std::string setting = "";
	if (ch->opt[MODE_i] == true)
		setting += "i";
	if (ch->opt[MODE_t] == true)
		setting += "t";
	if (ch->opt[MODE_k] == true)
		setting += "k";
	if (ch->opt[MODE_o] == true)
		setting += "o";
	if (ch->opt[MODE_l] == true)
		setting += "l";
	return (setting);
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