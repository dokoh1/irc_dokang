/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCMessageParse.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sihkang <sihkang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 16:27:19 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/12 13:32:00 by sihkang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "../IRCServer.hpp"

void IRCServer::IRCMessageParse(std::string message)
{
	std::stringstream ss;
	std::string 	  param;
	
	ss << message;
	parsedMessage.numParams = 0;
	if (ss.peek() == ':')
	{
		ss >> parsedMessage.prefix;
		parsedMessage.prefix.erase(1, 0);
		
		ss >> parsedMessage.command;		
		while (ss >> param)
		{
			parsedMessage.params.push_back(param);
			parsedMessage.numParams++;
		}
	}		
	else
	{
		parsedMessage.prefix = "";
		ss >> parsedMessage.command;		
		while (ss >> param)
		{
			parsedMessage.params.push_back(param);
			parsedMessage.numParams++;
		}
	}
}

// 닉네임 중복검사, 유효한 캐릭터로 구성되었는지.
