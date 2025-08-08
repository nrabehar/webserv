/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WException.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:29:40 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/08 11:42:51 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>
#include <iostream>

class WException : public std::exception
{
private:
	WException();
	WException &operator=(const WException &);

protected:
	std::string _msg;

public:
	WException(const WException &);
	WException(const std::string &msg);
	virtual ~WException() throw();
	virtual const char *what() const throw();
};

class ParseException : public WException
{
public:
	ParseException(const std::string &);
	ParseException(const ParseException &);
	virtual ~ParseException() throw();
};

class ValidationException : public WException
{
public:
	ValidationException(const std::string &);
	ValidationException(const ValidationException &);
	virtual ~ValidationException() throw();
};
#endif
