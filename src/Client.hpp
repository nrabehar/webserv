/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 23:27:50 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/09 01:24:07 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <unistd.h>

class Client
{
	private:
		int _fd;

	public:
		Client(int fd);
		~Client();

		int getFd() const;

	private:
		Client(const Client &);
		Client &operator=(const Client &);
};

#endif
