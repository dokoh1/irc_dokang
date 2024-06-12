/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMessage.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sihkang <sihkang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 14:11:25 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/12 18:14:11 by sihkang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void Response::checkMessage(int client_fd, IRCMessage message, serverInfo &info)
{
	std::cout << message.prefix << " | " << message.command << " | " ;
	int i = 0;
	while (i < message.numParams)
	{
		std::cout << message.params[i++] << " ";
	}
	std::cout << std::endl;

	if (isCommand(message, "JOIN"))
	{
		if (message.params[0] == ":")
			requestForJoin(client_fd);
		// else
			// join to channel
	}
	else if (isCommand(message, "PASS"))
	{
		if (!isCorrectPassword(info, message.params[0]))
		{
			send_message(client_fd, ":localhost 464 Error: Password incorrect.\n");
		}
		else
		{
			send_message(client_fd, "Password Correct! You are registered.\n");
			User *new_user = new User();
			new_user->client_fd = client_fd;
			info.usersInServer.push_back(new_user);
			
			// info.usersInServer.back()->username = message.params[0];
			// info.usersInServer.back()->hostname = message.params[1];
			// info.usersInServer.back()->servername = message.params[2];
			// info.usersInServer.back()->realname = message.params[3];
			
			send_message(client_fd, ":ft_irc.local 001 dokang :Welcome to the FT_IRC Network dokang!root@127.0.0.1\n");
			send_message(client_fd, ":irc.local 002 inspircd :Your host is irc.local, running version InspIRCd-3\n");
			send_message(client_fd, ":irc.local 003 inspircd :This server was created 03:34:23 Jun 08 2024\n");
			send_message(client_fd, ":irc.local 004 inspircd irc.local InspIRCd-3 iosw biklmnopstv :bklov\n");
			send_message(client_fd, ":irc.local 005 inspircd AWAYLEN=200 CASEMAPPING=rfc1459 CHANLIMIT=#:20 \n");
 
		}
	}
	else if (isCommand(message, "NICK"))
	{

		std::cout << "For debug : NICK \n";
		info.usersInServer.back()->nick = message.params[0];
	}
	else if (isCommand(message, "USER"))
	{
		std::cout << "For debug : USER \n";
		info.usersInServer.back()->username = message.params[0];
		info.usersInServer.back()->hostname = message.params[1];
		info.usersInServer.back()->servername = message.params[2];
		info.usersInServer.back()->realname = message.params[3];
		send_message(client_fd, ":ft_irc.local 001 dokang :Welcome to the FT_IRC Network dokang!root@127.0.0.1\n");
		// std::cout << info.usersInServer.back()->nick << " " <<
		// 		info.usersInServer.back()->username << " " <<
		// 		info.usersInServer.back()->hostname << " " <<
		// 		info.usersInServer.back()->servername << " " <<
		// 		info.usersInServer.back()->realname << std::endl;
	}
	else if (isCommand(message, "PRIVMSG"))
	{
		// 해당 채널에 있는 모든 사람들에게 메세지가 들어가도록.
		send_message(client_fd, "I GOT THE MESSAGE!\n");
	}
	else if (isCommand(message, "KICK"))
	{

	}
	else if (isCommand(message, "INVITE"))
	{

	}
	else if (isCommand(message, "TOPIC"))
	{

	}
	else if (isCommand(message, "MODE"))
	{

	}
	else if (isCommand(message, "WHOIS"))
	{
		Response::WHOIS(client_fd, findUser(info, message.params[0]));
	}
	else if (isCommand(message, "PING"))
	{
		send_message(client_fd, "PONG ft_irc local");
	}
	else
	{
		// wrong command
	}
	send_message(client_fd, "\r\n");
}

