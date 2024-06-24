/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_kick.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sihkang <sihkang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 18:47:37 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/24 11:20:03 by sihkang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

// void Response::KICK(int client_fd, IRCMessage message, serverInfo &info)
// {
// 	Channel &ch = findChannel(info, message.params[0].erase(0, 1));	
// 	User &user = findUser(ch, message.params[1]);
// 	std::string reason = "";
// 	if (message.numParams == 3)
// 		reason += message.params[2];

// 	// if (ch == *(info.channelInServer.end()))
// 	// {
// 	// 	 rpl error code
// 	// }

// 	// if (user = *(ch->channelUser.end()))
// 	// {
// 	// 	rpl error code
// 	// }
	
// 	for (std::list<User *>::iterator it = ch->channelUser.begin(); it != ch->channelUser.end(); ++it)
// 	{
// 		if ((*it)->nick == user->nick)
// 		{
// 			ch->channelUser.erase(it);
// 			return ;
// 		}
// 	}
	
// 	if (findOPUser(ch, user->client_fd) != *(ch->operator_user.end()))
// 	{
// 		for (std::list<User *>::iterator it = ch->operator_user.begin(); it != ch->operator_user.end(); ++it)
// 		{
// 			if ((*it)->nick == user->nick)
// 			{
// 				ch->operator_user.erase(it);
// 				return ;
// 			}
// 		}
// 	}
// }