/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_mode.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sihkang <sihkang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:13:57 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/22 16:24:04 by sihkang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void Response:: MODE(int client_fd, IRCMessage message, serverInfo &info)
{
	if (findChannel(info, message.params[0]).name == "")
		return ;
	Channel &ch = findChannel(info, message.params[0]);
	User &requestUser = findUser(ch, client_fd);

	if (message.numParams == 1) // 채널명만 왔을경우 . 채널정보 리턴
	{
		Response::getChannelInfo(client_fd, requestUser, ch);
	}
	else
	{
		if (findOPUser(ch, client_fd).nick == "")
		{
			send_message(client_fd, ":dokang 482 " + requestUser.nick + " #" + ch.name + " :You must be a channel op");
			send_message(client_fd, "\r\n");
			return ;
		}
		changeChannelMode(client_fd, ch, message);
		Response::ChannelModeToUser(client_fd, message, info);
	}
	send_message(client_fd, "\r\n");
}

void Response::ChannelModeToUser(int client_fd, IRCMessage message, serverInfo &info)
{
	User &sender = findUser(info, client_fd);
	Channel &receivedChannel = findChannel(info, message.params[0]);
	
	std::list<User>::iterator it;
	for (it = ++(receivedChannel.channelUser.begin()); it != receivedChannel.channelUser.end(); ++it)
	{
		userPrefix(sender, (*it).client_fd);
		send_message((*it).client_fd, " " + message.command + " #" + receivedChannel.name
					+ " :" + getChannelMode(receivedChannel));
		send_message((*it).client_fd, "\r\n");
	}
}



void Response::getChannelInfo(int client_fd, User &requestUser, Channel &ch)
{
	send_message(client_fd, ":dokang 324 " + requestUser.nick + " #" 
				+ ch.name + " :" + getChannelMode(ch) + "\n");
	send_message(client_fd, ":dokang 329 " + requestUser.nick + " #"
				+ ch.name + " :" + ch.createdTime + "\r\n");	
}
