/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sihkang <sihkang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 13:48:17 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/13 17:35:06 by sihkang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
/**
 *  for register the unknown user(client)
 *  JOIN 명령 시 사용자 등록되어있지 않다면 451 응답코드 + 데이터를 요청해야함.
 */

void Response::send_message(int client_fd, std::string message)
{
	if (write(client_fd, message.c_str(), message.size()) == -1)
		throw sendMessageException();
	return ;
}

void Response::requestForRegi(int client_fd)
{
	// 451 응답코드에 패스워드 요청.
	send_message(client_fd, ":irc.local 451 * JOIN :You have not registered.\n");
	send_message(client_fd, "PASS <password>\n");
	return ;
}

void Response::joinToChannel(int client_fd, IRCMessage message, serverInfo &info)
{
	std::string chName = message.params[0].erase(0, 1);
	User *requestUser = findUser(info, client_fd);
	// if (채널이 존재하지 않는 경우) -> 서버의 채널목록에 해당 채널을 추가
	if (findChannel(info, chName) == *(info.channelInServer.end()))
	{
		Channel *new_channel = new Channel();
		new_channel->name = chName;
		new_channel->channelUser.push_back(findUser(info, client_fd));
		new_channel->operator_user = new_channel->channelUser.front();
		info.channelInServer.push_back(new_channel);
		std::cout << "added channel: " << new_channel->name << '\n';
	}
	
	// (채널이 이미 존재하는 경우 혹은 채널 생성 후) -> 해당 채널 ㅈㅓㅇ보 리턴
	Response::userPrefix(info);
	send_message(requestUser->client_fd, " JOIN :" + chName);
	send_message(requestUser->client_fd, "\n:dokang 353 " 
				+ requestUser->nick + " = " + chName 
				+ " :hihia @hihi zz\n"); //	:irc.local 353 aa = #ch1 :@aa
	
	send_message(requestUser->client_fd, ":dokang 366 " 
				+ requestUser->nick + " " + chName
				+ " :End of /NAMES list."); //aa #ch1 :End of /NAMES list.
	send_message(requestUser->client_fd, "\r\n");
	
}

void Response::WHOIS(int client_fd, User *user)
{
	// userinserver 에서 해당 nick의 유저 찾아서 정보 리턴.
	send_message(client_fd, ":server 311 requester " + (user->nick) + ' ' + user->username + ' ' + user->hostname + " * " + user->realname + '\n');
	send_message(client_fd, ":server 318 requester " + (user->nick) + " :End of WHOIS list\n");
}

void Response::userPrefix(serverInfo &info)
{
	send_message(info.usersInServer.back()->client_fd, ":");
	send_message(info.usersInServer.back()->client_fd, info.usersInServer.back()->nick);
	send_message(info.usersInServer.back()->client_fd, "!");
	send_message(info.usersInServer.back()->client_fd, info.usersInServer.back()->username);
	send_message(info.usersInServer.back()->client_fd, "@");
	send_message(info.usersInServer.back()->client_fd, info.usersInServer.back()->hostname);	
}
