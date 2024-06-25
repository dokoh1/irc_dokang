/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sihkang <sihkang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 13:48:17 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/25 15:56:11 by sihkang          ###   ########seoul.kr  */
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
	send_message(client_fd, "PASS <password>\r\n");
	// send_message(client_fd, "NICK <nickname>\n");
	// send_message(client_fd, "USER <username> <hostname> <servername> :<realname>\r\n");
	return ;
}

void Response::joinToChannel(int client_fd, IRCMessage message, serverInfo &info)
{
	std::string chName = message.params[0].erase(0, 1);
	User &requestUser = findUser(info, client_fd);
	// Channel &requestedChannel = findChannel(info, chName);
	
	// if (채널이 존재하지 않는 경우) -> 서버의 채널목록에 해당 채널을 추가
	
	if (findChannel(info, chName).name == "")
	{
		Channel new_channel;
		User dummyUser;
		dummyUser.client_fd = -1;
		dummyUser.nick = "";
		
		new_channel.channelUser.push_back(dummyUser);
		new_channel.operator_user.push_back(dummyUser);

		new_channel.name = chName;
		new_channel.channelUser.push_back(requestUser);
		new_channel.operator_user.push_back(requestUser);
		new_channel.createdTime = getCreatedTimeUnix();
		new_channel.key = "";
		setChannelMode(new_channel, 0, 1, 0, 0, 0);
		info.channelInServer.push_back(new_channel);
	}
	

	Channel &requestedChannel = findChannel(info, chName);

	if (requestedChannel.opt[MODE_i] == true && findUser(requestedChannel, requestUser.nick).nick == "")
	{
		send_message(client_fd, ":dokang 473 " + requestUser.nick + " #" + chName + " :Cannot join channel (invite only)\r\n");
		return ;
	}
	else if (requestedChannel.opt[MODE_k] == true && (message.numParams < 2 || message.params[1] != requestedChannel.key))
	{
		send_message(client_fd, ":dokang 475 " + requestUser.nick + " #" + chName + " :Cannot join channel (incorrect channel key)\r\n");
		return ;
	} 
	else if (requestedChannel.opt[MODE_l] == true && requestedChannel.user_limit <= static_cast<int>(requestedChannel.channelUser.size()))
	{
		send_message(client_fd, ":dokang 471 " + requestUser.nick + " #" + chName + " :Cannot join channel (channel is full)\r\n");
		return ;
	} 

	// 접속하려는 채널에 처음 접속하는 경우 -> 서버 유저목록에 유저 추가
	if (requestUser.nick != findUser(requestedChannel, requestUser.nick).nick)
	{
		requestedChannel.channelUser.push_back(requestUser);
	}
	
	Response::userPrefix(requestUser, client_fd);
	send_message(requestUser.client_fd, " JOIN :#" + chName);
	send_message(requestUser.client_fd, "\n:dokang 353 "
				+ requestUser.nick + " = " + chName 
				+ " :" + channelUserList(requestedChannel) + '\n'); //	:irc.local 353 aa = #ch1 :@aa
	
	send_message(requestUser.client_fd, ":dokang 366 " 
				+ requestUser.nick + " " + chName
				+ " :End of /NAMES list."); //aa #ch1 :End of /NAMES list.
	send_message(requestUser.client_fd, "\r\n");

	for (std::list<User>::iterator it = ++(requestedChannel.channelUser.begin()) ; it != requestedChannel.channelUser.end(); ++it)
	{
		if (client_fd == (*it).client_fd)
			continue;
		userPrefix(requestUser, (*it).client_fd);
		send_message((*it).client_fd, " JOIN :#" + requestedChannel.name + "\r\n");
	}
	// Response::ToChannelUser(client_fd, message, info, false);

}

void Response::WHOIS(int client_fd, User &user)
{
	// userinserver 에서 해당 nick의 유저 찾아서 정보 리턴.
	send_message(client_fd, ":server 311 requester " + (user.nick) + ' ' + user.username + ' ' + user.hostname + " * " + user.realname + '\n');
	send_message(client_fd, ":server 318 requester " + (user.nick) + " :End of WHOIS list\r\n");
}

void Response::userPrefix(User &user, int receiveSocket)
{
	send_message(receiveSocket, ":");
	send_message(receiveSocket, user.nick);
	send_message(receiveSocket, "!");
	send_message(receiveSocket, user.username);
	send_message(receiveSocket, "@");
	send_message(receiveSocket, user.hostname);	
}

void Response::QUIT(int client_fd, serverInfo &info)
{
	User &quitUser = findUser(info, client_fd);

	std::list<Channel>::iterator chit;
	std::list<User>::iterator usrit;
	
	for (chit = ++(info.channelInServer.begin()); chit != info.channelInServer.end(); ++chit)
	{
		EraseOPInChannel(*chit, quitUser);
		EraseUserInChannel(*chit, quitUser, info);

		for (std::list<User>::iterator it = ++((*chit).channelUser.begin()) ; it != (*chit).channelUser.end(); ++it)
		{
			if (client_fd == (*it).client_fd)
				continue;
			userPrefix(quitUser, (*it).client_fd);
			send_message((*it).client_fd, " QUIT :Quit: leaving\r\n");
		}
	}
	for (usrit = ++(info.usersInServer.begin()); usrit != info.usersInServer.end(); ++usrit)
	{
		if ((*usrit).nick == quitUser.nick)
		{
			info.usersInServer.erase(usrit);
			break;
		}
	}

	std::cout << "*  * * * CHANNEL IN SERVER : ";

	for (std::list<Channel>::iterator it = info.channelInServer.begin(); it != info.channelInServer.end(); ++it)
		std::cout << (*it).name << " ";
	std::cout << '\n';


	return ;
}