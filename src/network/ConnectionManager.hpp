#ifndef	CONNECTION_MANAGER_HPP
#define	CONNECTION_MANAGER_HPP

#include "../client/ClientManager.hpp"
#include <vector>
#include <sys/socket.h>

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

		ssize_t	readClientData(int fd, char *buffer, ssize_t size);
		ssize_t	sendClientData(int fd, const char *data, ssize_t size);

	private:

		ConnectionManager(const ConnectionManager &);
		ConnectionManager&	operator=(const ConnectionManager &);

};

#endif
