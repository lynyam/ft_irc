#include "Client.hpp"

/*
 * IRC messages are limited to 512 bytes including the trailing CRLF.
 * Since popLine() removes CRLF before parsing, Parser must receive at most
 * 510 bytes.
 *
 * Modern IRC guidance allows several strategies for oversized input:
 * sending ERR_INPUTTOOLONG, truncating, ignoring, or closing the connection.
 * For this project, we choose to request disconnection on oversized input.
 * This keeps the input buffer bounded and avoids parsing invalid messages.
 */

Client::Client(int fd)
	: _fd(fd),
	  _inputBuffer(),
	  _outputBuffer(),
	  _passwordAccepted(false),
	  _registered(false),
	  _nickname(),
	  _username(),
	  _realname(),
	  _disconnectRequested(false)
{
}

Client::~Client()
{
}

void	Client::checkPendingLineLimit()
{
	std::string::size_type	newline;

	newline = _inputBuffer.find('\n');
	if (newline == std::string::npos
		&& _inputBuffer.size() >= IRC_MAX_MESSAGE_SIZE)
		requestDisconnect();
}

int	Client::getFd() const
{
	return (_fd);
}

void	Client::appendInput(const std::string& data)
{
	_inputBuffer += data;
	checkPendingLineLimit();
}

bool	Client::hasCompleteLine() const
{
	return (_inputBuffer.find('\n') != std::string::npos);
}

std::string	Client::popLine()
{
	std::string::size_type	pos;
	std::string				line;

	pos = _inputBuffer.find('\n');
	if (pos == std::string::npos)
		return ("");
	if (pos + 1 > IRC_MAX_MESSAGE_SIZE)
	{
		_inputBuffer.erase(0, pos + 1);
		requestDisconnect();
		return ("");
	}
	line = _inputBuffer.substr(0, pos);
	_inputBuffer.erase(0, pos + 1);
	checkPendingLineLimit();
	if (!line.empty() && line[line.size() - 1] == '\r')
		line.erase(line.size() - 1);
	if (line.size() > IRC_MAX_LINE_SIZE)
	{
		requestDisconnect();
		return ("");
	}
	return (line);
}

void	Client::appendOutput(const std::string& data)
{
	_outputBuffer += data;
}

bool	Client::hasPendingOutput() const
{
	return (!_outputBuffer.empty());
}

const std::string&	Client::getOutputBuffer() const
{
	return (_outputBuffer);
}

void	Client::consumeOutput(size_t size)
{
	if (size >= _outputBuffer.size())
		_outputBuffer.clear();
	else
		_outputBuffer.erase(0, size);
}

void	Client::setPasswordAccepted(bool value)
{
	_passwordAccepted = value;
}

bool	Client::isPasswordAccepted() const
{
	return (_passwordAccepted);
}

void	Client::setNickname(const std::string& nickname)
{
	_nickname = nickname;
}

void	Client::setUsername(const std::string& username)
{
	_username = username;
}

void	Client::setRealname(const std::string& realname)
{
	_realname = realname;
}

const std::string&	Client::getNickname() const
{
	return (_nickname);
}

const std::string&	Client::getUsername() const
{
	return (_username);
}

const std::string&	Client::getRealname() const
{
	return (_realname);
}

bool	Client::hasNickname() const
{
	return (!_nickname.empty());
}

bool	Client::hasUsername() const
{
	return (!_username.empty());
}

bool	Client::canRegister() const
{
	return (_passwordAccepted && hasNickname() && hasUsername());
}

bool	Client::isRegistered() const
{
	return (_registered);
}

void	Client::markRegistered()
{
	_registered = true;
}

void	Client::requestDisconnect()
{
	_disconnectRequested = true;
}

std::string	Client::getPrefix() const
{
	return (":" + _nickname + "!" + _username + "@localhost");
}

bool	Client::shouldDisconnect() const
{
	return (_disconnectRequested);
}