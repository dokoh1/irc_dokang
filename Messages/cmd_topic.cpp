/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_topic.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sihkang <sihkang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 16:03:41 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/25 17:11:11 by sihkang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void Response::TOPIC(int client_fd, IRCMessage message, serverInfo &info)
{
	std::string chName = message.params[0].erase(0, 1);
	Channel &ch = findChannel(info, chName);
	User &user = findUser(ch, client_fd);
	std::string chTopic = aftercolonConcat(message);

	std::cout << ch.name << " | " << ch.opt[MODE_t] << " | " << findOPUser(ch, client_fd).nick << "\n";
	if (ch.opt[MODE_t] == true && findOPUser(ch, client_fd).nick == "")
	{
		send_message(client_fd, ":dokang 482 " + user.nick + " #" + ch.name + " :You must be a channel op");
		send_message(client_fd, "\r\n");
		return ;
	}
	ch.topic = chTopic;
	// Response::userPrefix(findUser(info, client_fd), client_fd);
	// send_message(client_fd, " TOPIC #" + chName + " :" + chTopic + "\r\n");
	Response::ToChannelUser(client_fd, message, info, true);
}
