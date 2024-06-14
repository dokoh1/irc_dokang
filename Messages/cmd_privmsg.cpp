/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_privmsg.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sihkang <sihkang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 12:18:57 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/14 15:34:06 by sihkang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void Response::ToChannelUser(int client_fd, IRCMessage message, serverInfo &info)
{
	User *sender = findUser(info, client_fd);
	Channel *receivedChannel = findChannel(info, message.params[0]);
	
	std::list<User *>::iterator it;
	for (it = receivedChannel->channelUser.begin(); it != receivedChannel->channelUser.end(); ++it)
	{
		// std::cout << "Why\n";
		if (client_fd == (*it)->client_fd)
			continue;
		// std::cout << "OK\n";
		userPrefix(sender, (*it)->client_fd);
		send_message((*it)->client_fd, " " + message.command + " #" + receivedChannel->name
					+ " :" + aftercolonConcat(message));
		send_message((*it)->client_fd, "\r\n");
	}
}

// void Response::ToChannelUser()