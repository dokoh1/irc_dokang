/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_quit.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sihkang <sihkang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 16:04:35 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/27 16:38:28 by sihkang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "../Messages/Response.hpp"

void Response::QUIT(int client_fd, serverInfo &info)
{
	User &quitUser = findUser(info, client_fd);

	std::list<Channel>::iterator chit;
	std::list<User>::iterator usrit;
	if (info.channelInServer.size() == 1)
		return ;
	
	for (chit = ++(info.channelInServer.begin()); chit != info.channelInServer.end();)
	{
		std::list<Channel>::iterator tmp_it = chit++;
		std::string chName = (*tmp_it).name;
		if (findUser((*tmp_it), quitUser.nick).nick != "")
		{
			for (std::list<User>::iterator it = ++((*tmp_it).channelUser.begin()) ; it != (*tmp_it).channelUser.end(); ++it)
			{
				if (client_fd == (*it).client_fd)
					continue;
				userPrefix(quitUser, (*it).client_fd, info);
				send_message((*it).client_fd, " QUIT :Quit: leaving\r\n", info);
			}
		}
		else
		{
			continue;
		}
		EraseOPInChannel(*tmp_it, quitUser);
		EraseUserInChannel(*tmp_it, quitUser);
		EraseChannelInServer(*tmp_it, info);
	}
	// 빈채널 삭제

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
