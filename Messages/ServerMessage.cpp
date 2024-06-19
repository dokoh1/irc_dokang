/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMessage.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sihkang <sihkang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 14:11:25 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/19 20:01:39 by sihkang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void Response::checkMessage(int client_fd, IRCMessage message, serverInfo &info)
{
	
	if (isCommand(message, "JOIN"))
	{
		if (message.params[0] == ":")
			Response::requestForRegi(client_fd);
		else
			Response::joinToChannel(client_fd, message, info);
	}
	
	else if (isCommand(message, "PASS"))
	{
		if (!isCorrectPassword(info, message.params[0]))
		{
			send_message(client_fd, ":localhost 464 Error: Password .\n");
		}
		else
		{
			send_message(client_fd, "Password Correct! You are registered.\n");
			User *new_user = new User();
			new_user->client_fd = client_fd;
			info.usersInServer.push_back(new_user);
		}
	}

	else if (isCommand(message, "NICK"))
	{
		info.usersInServer.back()->nick = message.params[0];
	}

	else if (isCommand(message, "USER"))
	{
		info.usersInServer.back()->username = message.params[0];
		info.usersInServer.back()->hostname = message.params[1];
		info.usersInServer.back()->servername = message.params[2];
		info.usersInServer.back()->realname = message.params[3];
		
		send_message(client_fd, ":dokang 001 " + info.usersInServer.back()->nick + " :Welcome to the ft_irc Network dokang!\n");
		send_message(client_fd, ":dokang 002 " + info.usersInServer.back()->nick + " :Your host is ft_irc by dokang\n");
		send_message(client_fd, ":dokang 003 " + info.usersInServer.back()->nick + " :This server was created " + info.serverCreatedTime + '\n');
		send_message(client_fd, ":dokang 004 " + info.usersInServer.back()->nick + " dokang dokangv1 \"\" itkol :\n");
		send_message(client_fd, ":dokang 005 " + info.usersInServer.back()->nick + " CASEMAPPING=rfc1459 KICKLEN=255 :are supported by this server\n");
		send_message(client_fd, "\r\n");
	}
	
	else if (isCommand(message, "PRIVMSG"))
	{
		Response::ToChannelUser(client_fd, message, info, false);
	}

	// else if (isCommand(message, "KICK"))
	// {
	// 	Response::KICK(client_fd, message, info);
	// }

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
	else if (isCommand(message, "WHOIS"))
	{
		Response::WHOIS(client_fd, findUser(info, message.params[0]));
	}
	else if (isCommand(message, "PING"))
	{
		send_message(client_fd, "PONG ft_irc local\r\n");
	}
	else
	{
		// wrong command
	}
}

