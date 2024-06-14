/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_mode.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sihkang <sihkang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:13:57 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/14 16:34:52 by sihkang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void Response::MODE(int client_fd, IRCMessage message, serverInfo &info)
{
	Channel *ch = findChannel(info, message.params[0]);
	if (ch == *(info.channelInServer.end()))
		return ;
	User *requestUser = findUser(ch, client_fd);

	if (message.numParams == 1)
	{
		send_message(client_fd, ":dokang 324 " + requestUser->nick + " #" 
					+ ch->name + " :+" + getChannelMode(ch) + '\n');
		send_message(client_fd, ":dokang 329 " + requestUser->nick + " #"
					+ ch->name + " :" + ch->createdTime);
	}
	send_message(client_fd, "\r\n");
}