/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 01:16:22 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/09 02:41:28 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_MANAGER_HPP
#define CLIENT_MANAGER_HPP


#include "Client.hpp"
#include <map>

class	ClientManager
{

	private:

		std::map<int, Client *> _clients;

	public:

		ClientManager();
		~ClientManager();

		void	addClient(int fd);
		void	removeClient(int fd);
		Client	*getClient(int fd);

};

#endif