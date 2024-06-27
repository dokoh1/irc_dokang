/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_mode.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sihkang <sihkang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:13:57 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/27 19:18:31 by sihkang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "../Messages/Response.hpp"

void Response::MODE(int client_fd, IRCMessage message, serverInfo &info)
{
	User &usr = findUser(info, client_fd);
	if (message.params[0].front() != '#')
	{
		return ;
	}

	Channel &ch = findChannel(info, message.params[0].erase(0, 1));
	if (message.numParams == 1)
	{
		Response::getChannelInfo(client_fd, usr, ch, info);
	}
	else
	{
		if (message.params[1] == "b")
			return ;
		if (findOPUser(ch, client_fd).nick == "")
		{
			send_message(client_fd, ":dokang 482 " + usr.nick + " #" 
						+ ch.name + " :You must be a channel op\r\n", info);
			return ;
		}

		changeChannelMode(client_fd, ch, message, info);
	}
	send_message(client_fd, "\r\n", info);
}

void Response::ChannelModeToUser(int client_fd, IRCMessage message, Channel &ch, serverInfo &info)
{
	User &sender = findUser(ch, client_fd);
	
	std::list<User>::iterator it;
	std::string params = '#' + message.params[0];
	int i;
	for (i = 1; i < message.numParams - 1; i++)
	{
		params += " " + message.params[i];
	}
	params += " :" + message.params[i];

	for (it = ++(ch.channelUser.begin()); it != ch.channelUser.end(); ++it)
	{
		userPrefix(sender, (*it).client_fd, info);
		send_message((*it).client_fd, " " + message.command + " " + params + "\r\n", info);
	}
}

void Response::getChannelInfo(int client_fd, User &requestUser, Channel &ch, serverInfo &info)
{
	send_message(client_fd, ":dokang 324 " + requestUser.nick + " #" 
				+ ch.name + " :" + getChannelMode(ch) + "\n", info);
	send_message(client_fd, ":dokang 329 " + requestUser.nick + " #"
				+ ch.name + " :" + ch.createdTime + "\r\n", info);	
}
