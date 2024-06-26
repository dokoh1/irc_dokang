/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rpl.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sihkang <sihkang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 15:36:03 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/26 14:53:55 by sihkang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void Response::rpl_connection(int client_fd, User &user, serverInfo &info)
{
	if (user.nickComplete == true)
	{
		send_message(client_fd, ":dokang 001 " + user.nick + " :Welcome to the ft_irc Network dokang!\r\n");
		send_message(client_fd, ":dokang 002 " + user.nick + " :Your host is ft_irc by dokang\r\n");
		send_message(client_fd, ":dokang 003 " + user.nick + " :This server was created " + info.serverCreatedTime + "\r\n");
		send_message(client_fd, ":dokang 004 " + user.nick + " dokang dokangv1 io itkol :bklov\r\n");
		send_message(client_fd, ":dokang 005 " + user.nick + " CASEMAPPING=rfc1459 KICKLEN=255 :are supported by this server\r\n");
	}
}

void Response::rpl_passCorrect(int client_fd, serverInfo &info)
{
	send_message(client_fd, "Password Correct! Register user infomation \"NICK <nickname>\" \"USER <username> <hostname> <servername> :<realname>\" \r\n");
	User new_user;
	new_user.client_fd = client_fd;
	new_user.auth = true;
	new_user.nickComplete = false;
	info.usersInServer.push_back(new_user);
}

void Response::rpl461(int client_fd, User &user, IRCMessage message)
{
	send_message(client_fd, ":dokang 461 " + user.nick 
				+ message.command + " :Need more parameters\r\n");
}

void Response::rpl421(int client_fd, IRCMessage message)
{
	send_message(client_fd, ":dokang 421 :Unknown command \"" + message.command + "\"\r\n");
}

void Response::rpl442(int client_fd, User &user, std::string chName)
{
	send_message(client_fd, ":dokang 442 " + user.nick + " #" + chName 
				+ " :You are not on that channel.\r\n");
}

void Response::rpl464(int client_fd)
{
	send_message(client_fd, ":localhost 464 Error: Password incorrect.\r\n");
}

void Response::rpl482(int client_fd, User &user, std::string chName)
{
	send_message(client_fd, ":dokang 482 " + user.nick + " " + chName
		+ " :You must be a channel op.\r\n");
}

void Response::rpl441(int client_fd, User &user, IRCMessage message)
{
	send_message(client_fd, ":dokang 441 " + user.nick + " " + message.params[1] + " " + message.params[0]
				+ " :They are not on that channel\r\n");
}

void Response::rpl472(int client_fd, User &user, char wrongMode)
{
	send_message(client_fd, ":dokang 472 " + user.nick + " " 
				+ wrongMode + " :is not a recognised channel mode.\r\n");
}

void Response::rpl401_modeErr(int client_fd, User &OPuser, std::string targetUser)
{
		Response::send_message(client_fd, ":dokang 401 " + OPuser.nick + " "
							+ targetUser + " :No such nick\r\n");
}