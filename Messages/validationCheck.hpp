/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validationCheck.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sihkang <sihkang@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:16:44 by sihkang           #+#    #+#             */
/*   Updated: 2024/06/12 16:06:32 by sihkang          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALIDATIONCHECK_HPP
# define VALIDATIONCHECK_HPP

#include "../IRCServer.hpp"

struct IRCMessage;
struct serverInfo;

bool isCommand(IRCMessage msg, std::string cmd);
bool isCorrectPassword(serverInfo &info, std::string &client_pw);
User *findUser(serverInfo info, std::string nick);

#endif