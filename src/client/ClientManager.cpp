#include "ClientManager.hpp"

ClientManager::ClientManager() {}
ClientManager::~ClientManager()
{
	std::map<int, Client *>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); ++it)
		delete (*it).second;
	_clients.clear();
}

void ClientManager::addClient(int fd)
{
	Client *client = new Client(fd);
	_clients.insert(std::make_pair(fd, client));
}

void ClientManager::removeClient(int fd)
{
	std::map<int, Client *>::iterator it = _clients.find(fd);
	if (it != _clients.end())
	{
		delete it->second;
		_clients.erase(it);
	}
}

Client *ClientManager::getClient(int fd)
{
	std::map<int, Client *>::iterator it = _clients.find(fd);
	if (it != _clients.end())
		return ((*it).second);
	return (NULL);
}
