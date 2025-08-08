/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Checker.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:34:23 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/08 11:35:53 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHECKER_HPP
#define CHECKER_HPP

#include "String.hpp"

bool isNumeric(const std::string &);
bool isValidMethod(const std::string &);
bool isValidPort(size_t);
bool isValidIP(const std::string &);

#endif