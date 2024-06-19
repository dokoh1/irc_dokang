/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCMessageParse.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sihkang <sihkang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 16:27:19 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/19 19:59:36 by sihkang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "../IRCServer.hpp"

void IRCServer::IRCMessageParse(std::string message)
{
	std::stringstream ss;
	std::string		  cmd;
	std::string 	  param;
	
	ss << message;
	parsedMessage.command = "";
	parsedMessage.numParams = 0;
	if (ss.peek() == ':')
	{
		ss >> parsedMessage.prefix;
		parsedMessage.prefix.erase(0, 1);
		
		ss >> cmd;
		for (unsigned long i = 0; i < cmd.size(); i++)
		{
			if (isalpha(cmd[i]))
				parsedMessage.command += toupper(cmd[i]);
			else
				parsedMessage.command += cmd[i];
		}

		while (ss >> param)
		{
			parsedMessage.params.push_back(param);
			parsedMessage.numParams++;
		}
	}		
	else
	{
		parsedMessage.prefix = "";
		
		ss >> cmd;
		for (unsigned long i = 0; i < cmd.size(); i++)
		{
			if (isalpha(cmd[i]))
				parsedMessage.command += toupper(cmd[i]);
			else
				parsedMessage.command += cmd[i];
		}
		
		while (ss >> param)
		{
			parsedMessage.params.push_back(param);
			parsedMessage.numParams++;
		}
	}

	std::cout << parsedMessage.prefix << " | " << parsedMessage.command << " | " ;
	int i = 0;
	while (i < parsedMessage.numParams)
	{
		std::cout << parsedMessage.params[i++] << " ";
	}
	std::cout << std::endl;

}

// 닉네임 중복검사, 유효한 캐릭터로 구성되었는지.
