/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rpl.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sihkang <sihkang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 15:36:03 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/24 15:44:39 by sihkang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void Response::rpl461(int client_fd, User &user, IRCMessage message)
{
	send_message(client_fd, ":dokang 461 " + user.nick 
				+ message.command + " :Need more parameters\r\n");
}

void Response::rpl442(int client_fd, User &user, std::string chName)
{
	send_message(client_fd, ":dokang 442 " + user.nick + " #" + chName 
				+ " :You are not on that channel.\r\n");
}

void Response::rpl482(int client_fd, User &user, std::string chName)
{
	send_message(client_fd, ":dokang 482 " + user.nick + " " + chName
		+ " :You must be a channel op or higher to kick a more privileged user.\r\n");
}

void Response::rpl441(int client_fd, User &user, IRCMessage message)
{
	send_message(client_fd, ":dokang 441 " + user.nick + " " + message.params[1] + " " + message.params[0]
				+ " :They are not on that channel\r\n");
}