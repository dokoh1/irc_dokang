/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sihkang <sihkang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:16:44 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/13 16:19:40 by sihkang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOOLS_HPP
# define TOOLS_HPP

#include "../IRCServer.hpp"

struct IRCMessage;
struct serverInfo;
struct User;
struct Channel;

bool isCommand(IRCMessage msg, std::string cmd);
bool isCorrectPassword(serverInfo &info, std::string &client_pw);
User *findUser(serverInfo &info, std::string nick);
User *findUser(serverInfo &info, int client_fd);
Channel *findChannel(serverInfo &info, std::string chName);
std::string aftercolonConcat(IRCMessage message);

#endif