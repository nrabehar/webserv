/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrabehar <nrabehar@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 02:47:36 by nrabehar          #+#    #+#             */
/*   Updated: 2025/08/09 05:27:11 by nrabehar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	HTTP_SERVER_HPP
#define	HTTP_SERVER_HPP
#include "../conf/Config.hpp"
#include "../network/NetworkManager.hpp"
#include "../network/ConnectionManager.hpp"

class	HttpServer
{

	private:
		const Config&	_config;
		NetworkManager	*_net_manager;
		ConnectionManager	*_con_manager;


	public:

		HttpServer(const Config& config);
		~HttpServer();

		void	start();
		void	listen();

	private:


		void	readRequest(int fd);
		void	writeRequest(int fd);
		void	errorRequest(int fd);

		HttpServer();
    HttpServer(const HttpServer&);
    HttpServer& operator=(const HttpServer&);

};

#endif
