#include "ChannelManager.hpp"
#include "Client.hpp"
#include "ReplyBuilder.hpp"
#include <set>

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
	Channel* channel = get(name);
	if (exists(name))
		return (NULL);
	channel = new Channel(name);
	_channels[name] = channel;
	return (channel);
}

Channel*	ChannelManager::getOrCreate(const std::string& name)
{
	Channel* channel = get(name);
	if (channel)
		return (channel);
	return (create(name));
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
	std::map<std::string, Channel*>::iterator	it;
	std::map<std::string, Channel*>::iterator	next;
	Channel*	channel;
	bool		wasOperator;
	Client*		newOp;

	if (!client)
		return ;
	it = _channels.begin();
	while (it != _channels.end())
	{
		channel = it->second;
		wasOperator = channel->isOperator(client);
		channel->removeClient(client);
		if (channel->isEmpty())
		{
			next = it;
			++next;
			delete channel;
			_channels.erase(it);
			it = next;
		}
		else
		{
			if (wasOperator && !channel->hasOperator())
			{
				newOp = channel->getFirstMember();
				if (newOp) {
					channel->addOperator(newOp);
					channel->broadcast(ReplyBuilder::mode(
				"server", channel->getName(), "+o", newOp->getNickname()));
		
				}
			}
			++it;
		}
	}
}

void	ChannelManager::broadcastToClientChannels(Client* client, const std::string& message)
{
	std::map<std::string, Channel*>::iterator	it;
	std::set<Client*>							recipients;
	std::set<Client*>::const_iterator			clientIt;

	if (!client)
		return ;
	it = _channels.begin();
	while (it != _channels.end())
	{
		if (it->second && it->second->hasClient(client))
		{
			const std::set<Client*>& members = it->second->getClients();
			clientIt = members.begin();
			while (clientIt != members.end())
			{
				if (*clientIt != client)
					recipients.insert(*clientIt);
				++clientIt;
			}
		}
		++it;
	}
	clientIt = recipients.begin();
	while (clientIt != recipients.end())
	{
		(*clientIt)->appendOutput(message);
		++clientIt;
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
