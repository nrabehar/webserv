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