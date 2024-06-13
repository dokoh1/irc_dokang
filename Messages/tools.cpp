/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sihkang <sihkang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:16:22 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/13 17:15:20 by sihkang          ###   ########seoul.kr  */
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

Channel *findChannel(serverInfo &info, std::string chName)
{
	std::list<Channel *>::iterator it;
	
	std::cout << *info.channelInServer.begin() << " | " << *info.channelInServer.end() << '\n';
	for (it = info.channelInServer.begin(); it != info.channelInServer.end(); ++it)
	{
		if ((*it)->name == chName)
			return (*it);
	}
	return (*it);
}