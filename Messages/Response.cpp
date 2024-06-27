/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sihkang <sihkang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 13:48:17 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/27 18:15:01 by sihkang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void Response::send_message(int client_fd, std::string message, serverInfo &info)
{
	try
	{	
		if (write(client_fd, message.c_str(), message.size()) == -1)
			throw sendMessageException();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		Response::QUIT(client_fd, info);
	}
	return ;
}

void Response::requestForRegi(int client_fd, serverInfo &info)
{
	// 451 응답코드에 패스워드 요청.
	send_message(client_fd, ":irc.local 451 * JOIN :You have not registered.\n", info);
	send_message(client_fd, "PASS <password>\r\n", info);
	return ;
}

void Response::addNewChannel(User &requestUser, std::string chName, serverInfo &info)
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

void Response::joinToChannel(int client_fd, IRCMessage message, serverInfo &info)
{
	User &requestUser = findUser(info, client_fd);
	if (requestUser.nick == "" || !requestUser.auth || !requestUser.nickComplete || !requestUser.userComplete)
	{
		return ;
	}

	std::string chName = message.params[0].erase(0, 1);
		if (findChannel(info, chName).name == "")
	{
		addNewChannel(requestUser, chName, info);
	}
	
	Channel &requestedChannel = findChannel(info, chName);

	std::cout << "hahaha" << "\n";	
	if (requestedChannel.opt[MODE_i] == true && findUser(requestedChannel, requestUser.nick).nick == "")
	{
		send_message(client_fd, ":dokang 473 " + requestUser.nick + " #" + chName + " :Cannot join channel (invite only)\r\n", info);
		return ;
	}
	if (requestedChannel.opt[MODE_k] == true && (message.numParams < 2 || message.params[1] != requestedChannel.key))
	{
		send_message(client_fd, ":dokang 475 " + requestUser.nick + " #" + chName + " :Cannot join channel (incorrect channel key)\r\n", info);
		return ;
	} 
	if (requestedChannel.opt[MODE_l] == true && requestedChannel.user_limit + 1 <= static_cast<int>(requestedChannel.channelUser.size()))
	{
		send_message(client_fd, ":dokang 471 " + requestUser.nick + " #" + chName + " :Cannot join channel (channel is full)\r\n", info);
		return ;
	} 
	// 접속하려는 채널에 처음 접속하는 경우 -> 서버 유저목록에 유저 추가
	if (requestUser.nick != findUser(requestedChannel, requestUser.nick).nick)
	{
		requestedChannel.channelUser.push_back(requestUser);
	}
	
	Response::userPrefix(requestUser, client_fd, info);
	send_message(requestUser.client_fd, " JOIN :#" + chName, info);
	send_message(requestUser.client_fd, "\n:dokang 353 "
				+ requestUser.nick + " = " + chName 
				+ " :" + channelUserList(requestedChannel) + '\n', info);
	
	send_message(requestUser.client_fd, ":dokang 366 " 
				+ requestUser.nick + " " + chName
				+ " :End of /NAMES list.", info);
	send_message(requestUser.client_fd, "\r\n", info);

	for (std::list<User>::iterator it = ++(requestedChannel.channelUser.begin()) ; it != requestedChannel.channelUser.end(); ++it)
	{
		if (client_fd == (*it).client_fd)
			continue;
		userPrefix(requestUser, (*it).client_fd, info);
		send_message((*it).client_fd, " JOIN :#" + requestedChannel.name + "\r\n", info);
	}
}

void Response::userPrefix(User &user, int receiveSocket, serverInfo &info)
{
	send_message(receiveSocket, ":", info);
	send_message(receiveSocket, user.nick, info);
	send_message(receiveSocket, "!", info);
	send_message(receiveSocket, user.username, info);
	send_message(receiveSocket, "@", info);
	send_message(receiveSocket, user.hostname, info);	
}
