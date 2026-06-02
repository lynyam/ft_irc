#include "CommandMessage.hpp"

CommandMessage::CommandMessage()
	: _prefix(),
	  _command(),
	  _params(),
	  _trailing(),
	  _hasTrailing(false)
{
}

CommandMessage::~CommandMessage()
{
}

const std::string&	CommandMessage::getPrefix() const
{
	return (_prefix);
}

const std::string&	CommandMessage::getCommand() const
{
	return (_command);
}

const std::vector<std::string>&	CommandMessage::getParams() const
{
	return (_params);
}

const std::string&	CommandMessage::getTrailing() const
{
	return (_trailing);
}

void	CommandMessage::setPrefix(const std::string& prefix)
{
	_prefix = prefix;
}

void	CommandMessage::setCommand(const std::string& command)
{
	_command = command;
}

void	CommandMessage::addParam(const std::string& param)
{
	_params.push_back(param);
}

void	CommandMessage::setTrailing(const std::string& trailing)
{
	_trailing = trailing;
	_hasTrailing = true;
}

bool	CommandMessage::hasTrailing() const
{
	return (_hasTrailing);
}

bool	CommandMessage::hasParam(size_t index) const
{
	return (index < _params.size());
}

const std::string&	CommandMessage::getParam(size_t index) const
{
	return (_params[index]);
}

size_t	CommandMessage::paramCount() const
{
	return (_params.size());
}