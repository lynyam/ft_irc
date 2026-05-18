#include "Channel.hpp"

Channel::Channel(const std::string& name)
	: _name(name),
	  _topic(),
	  _hasTopic(false),
	  _clients(),
	  _operators(),
	  _invited(),
	  _inviteOnly(false),
	  _topicProtected(true),
	  _hasKey(false),
	  _key(),
	  _hasUserLimit(false),
	  _userLimit(0)
{
}

Channel::~Channel()
{
}

/*
 * TODO(Luli):
 * Implement channel state management:
 * - members
 * - operators
 * - invites
 * - topic
 * - modes: i, t, k, l
 *
 * Important invariants:
 * - operators must be members
 * - removeClient() must also remove from operators and invited
 * - broadcast() must use Client::appendOutput(), 
 * - Channel must never call send().
 */

const std::string&	Channel::getName() const
{
	return (_name);
}

void	Channel::addClient(Client* client)
{
	if (!isInvited(client->getFd()))
		_clients.push_back(client->getFd());
}

void	Channel::removeClient(Client* client)
{
	_clients.erase(client);
}

bool	Channel::hasClient(Client* client) const
{
	return (_clients.find(client) != _clients.end());
}

bool	Channel::isEmpty() const
{
	return (_clients.empty());
}

size_t	Channel::getClientCount() const
{
	return (_clients.size());
}

void	Channel::addOperator(Client* client)
{
	if (!isOperator(client->getFd()))
		_operators.push_back(client->getFd());
}

void	Channel::removeOperator(Client* client)
{
	_operators.erase(client);
}

bool	Channel::isOperator(Client* client) const
{
	return (_operators.find(client) != _operators.end());
}

void	Channel::invite(Client* client)
{
	_invited.insert(client);
}

void	Channel::removeInvite(Client* client)
{
	_invited.erase(client);
}

bool	Channel::isInvited(Client* client) const
{
	return (_invited.find(client) != _invited.end());
}

void	Channel::setTopic(const std::string& topic)
{
	_topic = topic;
	_hasTopic = true;
}

const std::string&	Channel::getTopic() const
{
	return (_topic);
}

bool	Channel::hasTopic() const
{
	return (_hasTopic);
}

void	Channel::setInviteOnly(bool value)
{
	_inviteOnly = value;
}

bool	Channel::isInviteOnly() const
{
	return (_inviteOnly);
}

void	Channel::setTopicProtected(bool value)
{
	_topicProtected = value;
}

bool	Channel::isTopicProtected() const
{
	return (_topicProtected);
}

void	Channel::setKey(const std::string& key)
{
	_key = key;
	_hasKey = true;
}

void	Channel::removeKey()
{
	_hasKey = false;
}

bool	Channel::hasKey() const
{
	return (_hasKey);
}

bool	Channel::checkKey(const std::string& key) const
{
	if (!_hasKey)
		return (true);
	return (_key == key);
}

void	Channel::setUserLimit(size_t limit)
{
	_userLimit = limit;
	_hasUserLimit = true;
}

void	Channel::removeUserLimit()
{
	_hasUserLimit = false;
}

bool	Channel::hasUserLimit() const
{
	return (_hasUserLimit);
}

bool	Channel::isFull() const
{
	if (!_hasUserLimit)
		return (false);
	return (_clients.size() >= _userLimit);
}

size_t	Channel::getUserLimit() const
{
	return (_userLimit);
}

void	Channel::broadcast(const std::string& message)
{
	for (std::set<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		(*it)->appendOutput(message);// check if there's appendOutput()
}

void	Channel::broadcastExcept(Client* excluded, const std::string& message)
{
	for (std::set<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (*it != excluded)
			(*it)->appendOutput(message);
	}
}

std::string	Channel::buildNamesList() const
{
	std::string list;
	for (std::set<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (!list.empty())
			list += " ";
		if (isOperator(*it))
			list += "@";
		list += (*it)->getNickname();
	}
	return (list);
}

std::string	Channel::buildModeString() const
{
	std::string mode = "+";
	if (_inviteOnly)
		mode += "i";
	if (_topicProtected)
		mode += "t";
	if (_hasKey)
		mode += "k";
	if (_hasUserLimit)
		mode += "l";
	return (mode);
}