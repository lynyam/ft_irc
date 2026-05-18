#include "ClientManager.hpp"

ClientManager::ClientManager()
	: _clients()
{
}

ClientManager::~ClientManager()
{
	std::map<int, Client*>::iterator	it;

	it = _clients.begin();
	while (it != _clients.end())
	{
		delete it->second;
		++it;
	}
	_clients.clear();
}

void	ClientManager::addClient(int fd)
{
	if (_clients.find(fd) != _clients.end())
		return ;
	_clients[fd] = new Client(fd);
}

void	ClientManager::removeClient(int fd)
{
	std::map<int, Client*>::iterator	it;

	it = _clients.find(fd);
	if (it == _clients.end())
		return ;
	delete it->second;
	_clients.erase(it);
}

Client*	ClientManager::getByFd(int fd)
{
	std::map<int, Client*>::iterator	it;

	it = _clients.find(fd);
	if (it == _clients.end())
		return (NULL);
	return (it->second);
}

const Client*	ClientManager::getByFd(int fd) const
{
	std::map<int, Client*>::const_iterator	it;

	it = _clients.find(fd);
	if (it == _clients.end())
		return (NULL);
	return (it->second);
}

Client*	ClientManager::getByNickname(const std::string& nickname)
{
	std::map<int, Client*>::iterator	it;

	it = _clients.begin();
	while (it != _clients.end())
	{
		if (it->second->getNickname() == nickname)
			return (it->second);
		++it;
	}
	return (NULL);
}

const Client*	ClientManager::getByNickname(
	const std::string& nickname) const
{
	std::map<int, Client*>::const_iterator	it;

	it = _clients.begin();
	while (it != _clients.end())
	{
		if (it->second->getNickname() == nickname)
			return (it->second);
		++it;
	}
	return (NULL);
}

bool	ClientManager::nicknameExists(const std::string& nickname) const
{
	return (getByNickname(nickname) != NULL);
}

std::map<int, Client*>&	ClientManager::getAll()
{
	return (_clients);
}

const std::map<int, Client*>&	ClientManager::getAll() const
{
	return (_clients);
}