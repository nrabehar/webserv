/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WException.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:47:45 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/08 11:42:38 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WException.hpp"

WException::WException(const std::string &msg) : _msg(msg) {}
WException::WException(const WException &src) : _msg(src._msg) {}
WException::~WException() throw() {}

const char *WException::what() const throw()
{
	return _msg.c_str();
}

ParseException::ParseException(const std::string &msg) : WException(msg) {}
ParseException::ParseException(const ParseException &src) : WException(src._msg) {}
ParseException::~ParseException() throw() {}

ValidationException::ValidationException(const std::string &msg) : WException(msg) {}
ValidationException::ValidationException(const ValidationException &src) : WException(src._msg) {}
ValidationException::~ValidationException() throw() {}
