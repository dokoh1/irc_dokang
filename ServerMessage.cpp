/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMessage.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sihkang <sihkang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 14:11:25 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/08 15:41:35 by sihkang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"

void IRCServer::checkMessage(int client_fd, std::string message)
{
	if (message.find("JOIN :") != std::string::npos)
	{
		std::cout << "For debug : got JOIN message\n";
		send_message(client_fd, "hi i am server sihkang\n");
		requestForJoin(client_fd);
	}
	else if (message.find("USER") != std::string::npos)
	{
		std::cout << "For debug : got USER data\n";
		send_message(client_fd, ":irc.local NOTICE inspircd :*** Could not resolve your hostname: Request timed out; using your IP address (127.0.0.1) instead.\n");
		send_message(client_fd, ":irc.local 001 inspircd :Welcome to the Localnet IRC Network inspircd!root@127.0.0.1\n");
		send_message(client_fd, ":irc.local 002 inspircd :Your host is irc.local, running version InspIRCd-3\n");
		send_message(client_fd, ":irc.local 003 inspircd :This server was created 03:34:23 Jun 08 2024\n");
		send_message(client_fd, ":irc.local 004 inspircd irc.local InspIRCd-3 iosw biklmnopstv :bklov\n");
		send_message(client_fd, ":irc.local 005 inspircd AWAYLEN=200 CASEMAPPING=rfc1459 CHANLIMIT=#:20 CHANMODES=b,k,l,imnpst CHANNELLEN=64 CHANTYPES=# ELIST=CMNTU HOSTLEN=64 KEYLEN=32 KICKLEN=255 LINELEN=512 MAXLIST=b:100 :are supported by this server\n");
		send_message(client_fd,":irc.local 005 inspircd MAXTARGETS=20 MODES=20 NAMELEN=128 NETWORK=Localnet NICKLEN=30 PREFIX=(ov)@+ SAFELIST STATUSMSG=@+ TOPICLEN=307 USERLEN=10 USERMODES=,,s,iow WHOX :are supported by this server\n");
		send_message(client_fd, ":irc.local 251 inspircd :There are 0 users and 0 invisible on 1 servers\n");
		send_message(client_fd,":irc.local 253 inspircd 1 :unknown connections\n");
		send_message(client_fd,":irc.local 254 inspircd 0 :channels formed\n");
		send_message(client_fd,":irc.local 255 inspircd :I have 0 clients and 0 servers\n");
		send_message(client_fd,":irc.local 265 inspircd :Current local users: 0  Max: 1\n");
		send_message(client_fd,":irc.local 266 inspircd :Current global users: 0  Max: 1\n");
		send_message(client_fd,":irc.local 375 inspircd :irc.local message of the day\n");
		send_message(client_fd,":irc.local 372 inspircd :(The sysadmin possibly wants to edit </etc/inspircd/inspircd.motd>)\n");
		send_message(client_fd,":irc.local 376 inspircd :End of message of the day.\n");
	}
}

/**
 *  for register the unknown user(client)
 *  JOIN 명령 시 사용자 등록되어있지 않다면 451 응답코드 + 데이터를 요청해야함.
 */
void IRCServer::requestForJoin(int client_fd)
{
	send_message(client_fd, ":irc.local 451 * JOIN :You have not registered.\n");
	send_message(client_fd, "NICK <nickname>\n");
	send_message(client_fd, "USER <username> <hostname> <servername>\n");
	send_message(client_fd, "PASS <password>\n");
	return ;
}

void IRCServer::send_message(int client_fd, std::string message)
{
	if (write(client_fd, message.c_str(), message.size()) == -1)
		throw sendMessageException();
	return ;
}