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
	(void)client;
}

void	Channel::removeClient(Client* client)
{
	(void)client;
}

bool	Channel::hasClient(Client* client) const
{
	(void)client;
	return (false);
}

bool	Channel::isEmpty() const
{
	return (true);
}

size_t	Channel::getClientCount() const
{
	return (0);
}

void	Channel::addOperator(Client* client)
{
	(void)client;
}

void	Channel::removeOperator(Client* client)
{
	(void)client;
}

bool	Channel::isOperator(Client* client) const
{
	(void)client;
	return (false);
}

void	Channel::invite(Client* client)
{
	(void)client;
}

void	Channel::removeInvite(Client* client)
{
	(void)client;
}

bool	Channel::isInvited(Client* client) const
{
	(void)client;
	return (false);
}

void	Channel::setTopic(const std::string& topic)
{
	(void)topic;
}

const std::string&	Channel::getTopic() const
{
	return (_topic);
}

bool	Channel::hasTopic() const
{
	return (false);
}

void	Channel::setInviteOnly(bool value)
{
	(void)value;
}

bool	Channel::isInviteOnly() const
{
	return (false);
}

void	Channel::setTopicProtected(bool value)
{
	(void)value;
}

bool	Channel::isTopicProtected() const
{
	return (true);
}

void	Channel::setKey(const std::string& key)
{
	(void)key;
}

void	Channel::removeKey()
{
}

bool	Channel::hasKey() const
{
	return (false);
}

bool	Channel::checkKey(const std::string& key) const
{
	(void)key;
	return (false);
}

void	Channel::setUserLimit(size_t limit)
{
	(void)limit;
}

void	Channel::removeUserLimit()
{
}

bool	Channel::hasUserLimit() const
{
	return (false);
}

bool	Channel::isFull() const
{
	return (false);
}

size_t	Channel::getUserLimit() const
{
	return (0);
}

void	Channel::broadcast(const std::string& message)
{
	(void)message;
}

void	Channel::broadcastExcept(Client* excluded, const std::string& message)
{
	(void)excluded;
	(void)message;
}

std::string	Channel::buildNamesList() const
{
	return ("");
}

std::string	Channel::buildModeString() const
{
	return ("+");
}

Client* Channel::getFirstMember() const
{
    if (_clients.empty())
        return NULL;
    return *_clients.begin();
}

Client* Channel::getFirstMemberExcept(Client* excluded) const
{
    for (std::set<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (*it != excluded)
            return *it;
    }
    return NULL;
}

bool Channel::hasOperator() const
{
    return !_operators.empty();
}