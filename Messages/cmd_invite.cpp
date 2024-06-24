/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_invite.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sihkang <sihkang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 14:35:37 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/21 18:04:03 by sihkang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void Response::INVITE(int client_fd, IRCMessage message, serverInfo &info)
{
	User &requestUser = findUser(info, client_fd);

	User &invitedUser = findUser(info, message.params[0]);
	if (findUser(info, message.params[0]).nick == "")
	{
		send_message(client_fd, ":dokang 401 " + requestUser.nick 
					+ " " + message.params[0] + " :No such nick\r\n");
		return ;
	}
	
	std::string chName = message.params[1];
	if (chName[0] == '#')
		chName = chName.erase(0, 1);

	Channel &ch = findChannel(info, chName);
	if (ch.name == "")
	{
		send_message(client_fd, ":dokang 403 " + requestUser.nick 
					+ " #" + message.params[1] + " :No such channel\r\n");
		return ;
	}

	if (findUser(ch, invitedUser.nick).nick != "")
	{
		send_message(client_fd, ":dokang 443 " + requestUser.nick + " " 
					+ getMessageParams(message) +  + " :is already on channel\r\n");
		return ;
	}
	
	ch.channelUser.push_back(invitedUser);
	send_message(client_fd, ":dokang 341 " + requestUser.nick + " " + invitedUser.nick + " :#" + ch.name + "\r\n");
	userPrefix(requestUser, invitedUser.client_fd);
	send_message(invitedUser.client_fd, " INVITE " + invitedUser.nick + " :#" + ch.name + "\r\n");
}