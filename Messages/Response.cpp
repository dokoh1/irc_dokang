/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sihkang <sihkang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 13:48:17 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/12 18:06:15 by sihkang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
/**
 *  for register the unknown user(client)
 *  JOIN 명령 시 사용자 등록되어있지 않다면 451 응답코드 + 데이터를 요청해야함.
 */
void Response::requestForJoin(int client_fd)
{
	// 451 응답코드에 패스워드 요청.
	send_message(client_fd, ":irc.local 451 * JOIN :You have not registered.\n");
	send_message(client_fd, "PASS <password>\n");
	return ;
}

void Response::send_message(int client_fd, std::string message)
{
	if (write(client_fd, message.c_str(), message.size()) == -1)
		throw sendMessageException();
	return ;
}



void Response::WHOIS(int client_fd, User *user)
{
	// userinserver 에서 해당 nick의 유저 찾아서 정보 리턴.
	send_message(client_fd, ":server 311 requester " + (user->nick) + ' ' + user->username + ' ' + user->hostname + " * " + user->realname + '\n');
	// send_message(client_fd, ":server 319 requester " + (user->nick) + ":\n");
	// send_message(client_fd, ":server 312 requester " + (user->nick) + " " + (user->servername) + ":\n");
	// send_message(client_fd, ":server 317 requester " + (user->nick) + " 10 1614791234 :seconds idle\n");
	send_message(client_fd, ":server 318 requester " + (user->nick) + " :End of WHOIS list\n");
}



/*
WHOIS example
:server 311 requester [example] ~[user] [hostname] * :[Real Name]
:server 319 requester [example] :[#channel1 #channel2]
:server 312 requester [example] [servername] :[Server Info]
:server 317 requester [example] [10] [1614791234] :[seconds idle]
:server 318 requester [example] :End of WHOIS list
*/