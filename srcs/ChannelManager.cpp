#include "ChannelManager.hpp"

ChannelManager::ChannelManager()
	: _channels()
{
}

ChannelManager::~ChannelManager()
{
	std::map<std::string, Channel*>::iterator	it;

	it = _channels.begin();
	while (it != _channels.end())
	{
		delete it->second;
		++it;
	}
	_channels.clear();
}

/*
 * TODO(Luli):
 * Implement channel collection ownership:
 * - get()
 * - create()
 * - getOrCreate()
 * - remove()
 * - removeIfEmpty()
 * - removeClientFromAllChannels()
 *
 * Important:
 * removeClientFromAllChannels() is required by Server::disconnectClient()
 * before a Client* is deleted.
 */

Channel*	ChannelManager::get(const std::string& name)
{
	(void)name;
	return (NULL);
}

const Channel*	ChannelManager::get(const std::string& name) const
{
	(void)name;
	return (NULL);
}

Channel*	ChannelManager::create(const std::string& name)
{
	(void)name;
	return (NULL);
}

Channel*	ChannelManager::getOrCreate(const std::string& name)
{
	(void)name;
	return (NULL);
}

bool	ChannelManager::exists(const std::string& name) const
{
	(void)name;
	return (false);
}

void	ChannelManager::remove(const std::string& name)
{
	(void)name;
}

void	ChannelManager::removeIfEmpty(const std::string& name)
{
	(void)name;
}

void	ChannelManager::removeClientFromAllChannels(Client* client)
{
	(void)client;
}

void	ChannelManager::broadcastToClientChannels(Client* client, const std::string& message)
{
	std::map<std::string, Channel*>::iterator it;

	it = _channels.begin();
	while (it != _channels.end())
	{
		if (it->second && it->second->hasClient(client))
			it->second->broadcastExcept(client, message);
		++it;
	}
}

std::map<std::string, Channel*>&	ChannelManager::getAll()
{
	return (_channels);
}

const std::map<std::string, Channel*>&	ChannelManager::getAll() const
{
	return (_channels);
}