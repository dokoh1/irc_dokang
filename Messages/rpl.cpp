/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rpl.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sihkang <sihkang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 15:36:03 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/27 19:18:35 by sihkang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void Response::rpl_connection(int client_fd, User &user, serverInfo &info)
{
	if (user.nickComplete == true)
	{
		send_message(client_fd, ":dokang 001 " + user.nick + " :Welcome to the ft_irc Network dokang!\r\n", info);
		send_message(client_fd, ":dokang 002 " + user.nick + " :Your host is ft_irc by dokang\r\n", info);
		send_message(client_fd, ":dokang 003 " + user.nick + " :This server was created " + info.serverCreatedTime + "\r\n", info);
		send_message(client_fd, ":dokang 004 " + user.nick + " dokang dokangv1 io itkol :bklov\r\n", info);
		send_message(client_fd, ":dokang 005 " + user.nick + " CASEMAPPING=rfc1459 KICKLEN=255 :are supported by this server\r\n", info);
	}
}

void Response::rpl_passCorrect(int client_fd, serverInfo &info)
{
	send_message(client_fd, "Password Correct! Register user infomation \"NICK <nickname>\" \"USER <username> <hostname> <servername> :<realname>\" \r\n", info);
	User new_user;
	new_user.client_fd = client_fd;
	new_user.auth = true;
	new_user.nickComplete = false;
	new_user.userComplete = false;
	info.usersInServer.push_back(new_user);
}

void Response::rpl461(int client_fd, User &user, IRCMessage message, serverInfo &info)
{
	send_message(client_fd, ":dokang 461 " + user.nick + " "
				+ message.command + " :Need more parameters\r\n", info);
}

void Response::rpl421(int client_fd, serverInfo &info)
{
	send_message(client_fd, ":dokang 421 \r\n", info);
}

void Response::rpl442(int client_fd, User &user, std::string chName, serverInfo &info)
{
	send_message(client_fd, ":dokang 442 " + user.nick + " #" + chName 
				+ " :You are not on that channel.\r\n", info);
}

void Response::rpl464(int client_fd, serverInfo &info)
{
	send_message(client_fd, ":localhost 464 Error: Password incorrect.\r\n", info);
}

void Response::rpl482(int client_fd, User &user, std::string chName, serverInfo &info)
{
	send_message(client_fd, ":dokang 482 " + user.nick + " " + chName
		+ " :You must be a channel op.\r\n", info);
}

void Response::rpl441(int client_fd, User &user, IRCMessage message, serverInfo &info)
{
	send_message(client_fd, ":dokang 441 " + user.nick + " " + message.params[1] + " " + message.params[0]
				+ " :They are not on that channel\r\n", info);
}

void Response::rpl472(int client_fd, User &user, char wrongMode, serverInfo &info)
{
	send_message(client_fd, ":dokang 472 " + user.nick + " " 
				+ wrongMode + " :is not a recognised channel mode.\r\n", info);
}

void Response::rpl401_modeErr(int client_fd, User &OPuser, std::string targetUser, serverInfo &info)
{
		Response::send_message(client_fd, ":dokang 401 " + OPuser.nick + " "
							+ targetUser + " :No such nick\r\n", info);
}