/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   String.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:24:58 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/08 11:36:35 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRING_HPP
#define STRING_HPP

#include <iostream>
#include <vector>
#include <sstream>
#include <cstdlib>

std::string trimStr(const std::string &);

std::vector<std::string> splitStr(const std::string &, char delim);

template <typename T>
std::string toString(const T &val)
{
	std::ostringstream ostr;
	ostr << val;
	return (ostr.str());
}

size_t toBytes(const std::string &);

#endif
