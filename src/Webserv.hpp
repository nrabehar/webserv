/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:43:47 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/09 02:16:33 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "Config.hpp"

#include "Socket.hpp"

#include "Signal.hpp"

#include "EventManager.hpp"

#include "ClientManager.hpp"

class WebServ : public IEventHandler
{
	private:
		Config _conf;
		bool _running;
		std::vector<Socket> _sockets;
		std::vector<int> _new_clients;
		std::vector<int> _client_to_removes;
		EventManager _event_manager;
		ClientManager _client_manager;

	public:
		~WebServ();
		WebServ(const std::string &);

		void 	run();
		void 	createSocket(const Server &);
		void	cleanupClient(int fd);

		void	processEvents(const std::vector<EventInfo>& events);
		void 	onRead(int fd);
		void 	onWrite(int fd);
		void 	onError(int fd);

	private:
		WebServ();
		WebServ(const WebServ &);
		WebServ &operator=(const WebServ &);
};

#endif
