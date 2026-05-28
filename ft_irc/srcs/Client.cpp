#include "Client.hpp"

Client::Client(int fd)
	: _fd(fd),
	  _inputBuffer(),
	  _outputBuffer(),
	  _passwordAccepted(false),
	  _registered(false),
	  _nickname(),
	  _username(),
	  _realname()
{
}

Client::~Client()
{
}

int	Client::getFd() const
{
	return (_fd);
}

void	Client::appendInput(const std::string& data)
{
	_inputBuffer += data;
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
	line = _inputBuffer.substr(0, pos);
	if (!line.empty() && line[line.size() - 1] == '\r')
		line.erase(line.size() - 1);
	_inputBuffer.erase(0, pos + 1);
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

std::string	Client::getPrefix() const
{
	return (":" + _nickname + "!" + _username + "@localhost");
}