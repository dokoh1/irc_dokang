/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMessage.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dokoh <dokoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 14:11:25 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/24 11:53:13 by dokoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void Response::checkMessage(int client_fd, IRCMessage message, serverInfo &info)
{
	
	if (isCommand(message, "JOIN"))
	{
		std::cout << "CHANNEL LIST : ";
		for (std::list<Channel>::iterator it = info.channelInServer.begin(); it != info.channelInServer.end(); ++it)
		{
			std::cout << (*it).name << "|";
		}
		std::cout << "\n";
		if (message.params[0] == ":")
			Response::requestForRegi(client_fd);
		else
			Response::joinToChannel(client_fd, message, info);
	}
	
	else if (isCommand(message, "PASS"))
	{
		if (!isCorrectPassword(info, message.params[0]))
		{
			send_message(client_fd, ":localhost 464 Error: Password incorrect.\r\n");
		}
		else
		{
			send_message(client_fd, "Password Correct! Register user infomation \"NICK <nickname>\" \"USER <username> <hostname> <servername> :<realname>\" \r\n");
			User new_user;
			new_user.client_fd = client_fd;
			new_user.auth = true;
			info.usersInServer.push_back(new_user);
		}
	}

	else if (isCommand(message, "NICK"))
	{
		User &user = findUser(info, client_fd);
		
		if (user.client_fd > 2 && user.auth == true)
		{
			std::cout << user.nick << " | " << user.client_fd << " | " << user.auth << message.params[0] << "\n";
			user.nick = message.params[0];
		}
	}

	else if (isCommand(message, "USER"))
	{
		User &user = findUser(info, client_fd);
		std::cout << "DeBug Test\n";
		std::cout << user.nick << " | " << user.auth << "\n";

		if (user.nick != "" && user.auth == true)
		{	
			std::cout << "zz\n";
			user.username = message.params[0];
			user.hostname = message.params[1];
			user.servername = message.params[2];
			user.realname = message.params[3];
			
			std::cout << "SIBAL : "<< user.nick << " | " << user.username << " | " << user.hostname << " | " << user.servername << " | " << user.realname << "\n";
			send_message(client_fd, ":dokang 001 " + user.nick + " :Welcome to the ft_irc Network dokang!\n");
			send_message(client_fd, ":dokang 002 " + user.nick + " :Your host is ft_irc by dokang\n");
			send_message(client_fd, ":dokang 003 " + user.nick + " :This server was created " + info.serverCreatedTime + '\n');
			send_message(client_fd, ":dokang 004 " + user.nick + " dokang dokangv1 \"\" itkol :\n");
			send_message(client_fd, ":dokang 005 " + user.nick + " CASEMAPPING=rfc1459 KICKLEN=255 :are supported by this server\n");
			send_message(client_fd, "\r\n");
		}
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

