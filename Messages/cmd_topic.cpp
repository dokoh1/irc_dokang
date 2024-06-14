/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_topic.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sihkang <sihkang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 16:03:41 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/14 13:24:28 by sihkang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void Response::TOPIC(int client_fd, IRCMessage message, serverInfo &info)
{
	std::string chName = message.params[0];
	std::string chTopic = aftercolonConcat(message);
	findChannel(info, chName)->topic = chTopic;
	Response::userPrefix(findUser(info, client_fd), client_fd);
	send_message(client_fd, " TOPIC #" + chName + " :" + chTopic + "\r\n");
	Response::ToChannelUser(client_fd, message, info);
}
