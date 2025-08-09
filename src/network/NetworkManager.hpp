/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NetworkManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 03:02:14 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/09 04:16:52 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	NETWORK_MANAGER_HPP
#define	NETWORK_MANAGER_HPP

#include "../event/EventManager.hpp"

class	NetworkManager
{

	private:

		EventManager	_event_manager;
		std::vector<Socket> _sockets;
		std::vector<int> _pending_closes;

	public:

		NetworkManager();
		~NetworkManager();

		void	createSocket(const Server &server);
		std::vector<EventInfo>	getEvents();

		int	accept(int fd);
		bool	isServer(int fd);

		void	disconnect(int fd);
		void	cleanup();

};


#endif
