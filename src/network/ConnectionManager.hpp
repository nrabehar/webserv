/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionManager.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 03:22:08 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/09 04:47:17 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	CONNECTION_MANAGER_HPP
#define	CONNECTION_MANAGER_HPP

#include "../client/ClientManager.hpp"
#include <vector>

class	ConnectionManager
{

	private:

		ClientManager	_client_manager;
		std::vector<int>	_pending_closes;

	public:

		ConnectionManager();
		~ConnectionManager();

		void	connect(int c_fd);
		void	disconnect(int c_fd);
		void	cleanup();

		Client	*getClient(int fd);

	private:

		ConnectionManager(const ConnectionManager &);
		ConnectionManager&	operator=(const ConnectionManager &);

};

#endif
