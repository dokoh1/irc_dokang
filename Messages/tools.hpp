/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sihkang <sihkang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:16:44 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/26 18:20:28 by sihkang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOOLS_HPP
# define TOOLS_HPP

#include "../IRCServer.hpp"
#include <chrono>
#include <ctime>

struct IRCMessage;
struct serverInfo;
struct User;
struct Channel;

bool isCommand(IRCMessage msg, std::string cmd);
bool isCorrectPassword(serverInfo &info, std::string &client_pw);
User& findUser(serverInfo &info, std::string nick);
User& findUser(serverInfo &info, int client_fd);
User& findUser(Channel &ch, int client_fd);
User& findUser(Channel &ch, std::string nick);
User& findOPUser(Channel &ch, int client_fd);
User& findOPUser(Channel &ch, std::string nick);
Channel& findChannel(serverInfo &info, std::string chName);
std::string aftercolonConcat(IRCMessage message);
std::string channelUserList(Channel &requestedChannel);
void setChannelMode(Channel& ch, bool i, bool t, bool k, bool o, bool l);
std::string getChannelMode(Channel &ch);
std::string getCreatedTimeUnix();
std::string getCreatedTimeReadable();
void changeChannelMode(int client_fd, Channel &ch, IRCMessage msg);
std::string getMessageParams(IRCMessage message);

void modifyChannelOpt(int client_fd, Channel &ch, IRCMessage msg);
void unsetChannelOpt(int client_fd, Channel &ch, IRCMessage msg);
void EraseUserInChannel(Channel &ch, User &usr);
void EraseOPInChannel(Channel &ch, User &usr);
void EraseChannelInServer(Channel &ch, serverInfo &info);

#endif