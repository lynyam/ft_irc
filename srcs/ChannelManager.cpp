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
	if (name.empty())
		return (NULL);
	std::map<std::string, Channel*>::iterator	it = _channels.find(name);
	if (it == _channels.end())
		return (NULL);
	return (it->second);
}

const Channel*	ChannelManager::get(const std::string& name) const
{
	if (name.empty())
		return (NULL);
	std::map<std::string, Channel*>::const_iterator	it = _channels.find(name);
	if (it == _channels.end())
		return (NULL);
	return (it->second);
}

Channel*	ChannelManager::create(const std::string& name)
{
	if (name.empty())
		return (NULL);
	std::map<std::string, Channel*>::iterator	it = _channels.find(name);
	if (it != _channels.end())
		return (NULL);
	Channel* channel = new Channel(name);
	_channels[name] = channel;
	return (channel);
}

Channel*	ChannelManager::getOrCreate(const std::string& name)
{
	if (name.empty())
		return (NULL);
	std::map<std::string, Channel*>::iterator	it = _channels.find(name);
	if (it != _channels.end())
		return (it->second);
	Channel* channel = new Channel(name);
	_channels[name] = channel;
	return (channel);
}

bool	ChannelManager::exists(const std::string& name) const
{
	if (name.empty())
		return (false);
	return (_channels.find(name) != _channels.end());
}

void	ChannelManager::remove(const std::string& name)
{
	std::map<std::string, Channel*>::iterator	it = _channels.find(name);
	if (it == _channels.end())
		return ;
	delete it->second;
	_channels.erase(it);
}

void	ChannelManager::removeIfEmpty(const std::string& name)
{
	std::map<std::string, Channel*>::iterator	it = _channels.find(name);
	if (it == _channels.end())
		return ;
	if (it->second->isEmpty())
	{
		delete it->second;
		_channels.erase(it);
	}
}

void	ChannelManager::removeClientFromAllChannels(Client* client)
{
	std::map<std::string, Channel*>::iterator	it = _channels.begin();
	while (it != _channels.end())
	{
		it->second->removeClient(client);
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