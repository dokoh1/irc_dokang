/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validationCheck.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sihkang <sihkang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:16:22 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/12 16:06:22 by sihkang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "validationCheck.hpp"

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

User *findUser(serverInfo info, std::string nick)
{
	std::vector<User *>::iterator it;
	for (it = info.usersInServer.begin(); it != info.usersInServer.end(); ++it)
	{
		if ((*it)->nick == nick)
			return (*it);
	}
	return (*it);
}