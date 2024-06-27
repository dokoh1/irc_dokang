/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_privmsg.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sihkang <sihkang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 12:18:57 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/27 19:18:32 by sihkang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "../Messages/Response.hpp"

void Response::ToChannelUser(int client_fd, IRCMessage message, serverInfo &info, bool includeMe)
{
	User &sender = findUser(info, client_fd);
	std::string chName;
	if (message.params[0].front() == '#')
		chName = message.params[0].erase(0,1);
	else
		chName = message.params[0];
	Channel& receivedChannel = findChannel(info, chName);

	std::list<User>::iterator it;
	for (it = ++(receivedChannel.channelUser.begin()); it != receivedChannel.channelUser.end(); ++it)
	{
		if (client_fd == (*it).client_fd && includeMe == false)
			continue;
		userPrefix(sender, (*it).client_fd, info);
		send_message((*it).client_fd, " " + message.command + " #" + receivedChannel.name
					+ " :" + aftercolonConcat(message) + "\r\n", info);
	}
}