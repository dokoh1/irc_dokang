/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMessage.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sihkang <sihkang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 14:11:25 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/26 17:51:12 by sihkang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void Response::checkMessage(int client_fd, IRCMessage message, serverInfo &info)
{
	
	std::cout << "* * * * CHANNEL IN SERVER : ";

	for (std::list<Channel>::iterator it = info.channelInServer.begin(); it != info.channelInServer.end(); ++it)
		std::cout << (*it).name << "|";
	std::cout << '\n';

	if (isCommand(message, "JOIN"))
	{
		if (message.params[0] == ":")
			Response::requestForRegi(client_fd);
		else
			Response::joinToChannel(client_fd, message, info);
	}
	
	else if (isCommand(message, "PASS"))
	{
		if (message.numParams == 1 && isCorrectPassword(info, message.params[0]))
		{
			rpl_passCorrect(client_fd, info);
		}
		else
		{
			rpl464(client_fd);
		}
	}
	else if (isCommand(message, "NICK"))
	{
		User &user = findUser(info, client_fd);
		if (user.client_fd > 2 && user.auth == true)
		{
			std::cout << user.nick << " | " << user.client_fd << " | " << user.auth << message.params[0] << "\n";
			if (findUser(info, message.params[0]).nick != "")
				send_message(client_fd, ":dokang 433 * " + message.params[0] + " :Nickname is already in use.\r\n");
			else
			{
				user.nick = message.params[0];
				user.nickComplete = true;
				if (user.nickComplete && user.userComplete)
					rpl_connection(client_fd, user, info);
			}
		}
	}
	else if (isCommand(message, "USER"))
	{
		User &user = findUser(info, client_fd);
		std::cout << user.nick << " | " << user.auth << "\n";

		if (user.auth == true)
		{
			user.username = message.params[0];
			user.hostname = message.params[1];
			user.servername = message.params[2];
			user.realname = message.params[3];
			user.userComplete = true;
			if (user.nickComplete && user.userComplete)
				rpl_connection(client_fd, user, info);
		}
	}
	else if (isCommand(message, "PRIVMSG"))
	{
		Response::ToChannelUser(client_fd, message, info, false);
	}
	else if (isCommand(message, "KICK"))
	{
		Response::KICK(client_fd, message, info);
	}
	else if (isCommand(message, "INVITE"))
	{
		Response::INVITE(client_fd, message, info);
	}
	else if (isCommand(message, "TOPIC"))
	{
		Response::TOPIC(client_fd, message, info);
	}
	else if (isCommand(message, "MODE"))
	{
		Response::MODE(client_fd, message, info);
	}
	else if (isCommand(message, "PING"))
	{
		send_message(client_fd, "PONG ft_irc local\r\n");
	}
	else if (isCommand(message, "QUIT"))
	{
		Response::QUIT(client_fd, info);
	}
	else
	{
		Response::rpl421(client_fd);
	}
}

