#include "Server.hpp"
#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

Server::Server(int port, const std::string& password)
	: _port(port),
	  _password(password),
	  _serverFd(-1),
	  _running(false),
	  _clients(),
	  _channels(),
	  _dispatcher(_clients, _channels, _password)
{
}

Server::~Server()
{
	if (_serverFd >= 0)
		close(_serverFd);
}

void	Server::run()
{
	std::cout << "ircserv starting on port " << _port << std::endl;
	_running = true;
	/* TODO(Leon): 
    initSocket(); */
	/* eventLoop(); */
}

void	Server::stop()
{
	_running = false;
}

void	Server::initSocket()
{
    /* TODO(Leon):
	 * - socket()
	 * - setsockopt(SO_REUSEADDR)
	 * - bind()
	 * - listen()
	 * - setNonBlocking(_serverFd)
	 */
}

void	Server::setNonBlocking(int fd)
{
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("fcntl failed");
}

void	Server::eventLoop()
{
    /* TODO(Leon):
	 * - build readSet/writeSet
	 * - call select()
	 * - accept readable serverFd
	 * - recv readable client fds
	 * - send writable client fds
	 */
}

void	Server::prepareReadSet(fd_set& readSet, int& maxFd)
{
	(void)readSet;
	(void)maxFd;
}

void	Server::prepareWriteSet(fd_set& writeSet, int& maxFd)
{
	(void)writeSet;
	(void)maxFd;
}

void	Server::handleReadableFds(fd_set& readSet, int maxFd)
{
	(void)readSet;
	(void)maxFd;
}

void	Server::handleWritableFds(fd_set& writeSet, int maxFd)
{
	(void)writeSet;
	(void)maxFd;
}

void	Server::acceptClient()
{
    /* TODO(Leon):
	 * - accept()
	 * - set client fd non-blocking
	 * - _clients.addClient(clientFd)
	 */
}

void	Server::readFromClient(int fd)
{
	Client* client = _clients.getByFd(fd);
	if (!client || client->isDisconnectRequested())
		return;
    /* TODO(Leon):
	 * - recv() into a buffer
	 * - if n > 0: client->appendInput(), then while hasCompleteLine(): dispatcher.dispatch(*client, client->popLine())
	 * - if n == 0 or n < 0: disconnectClient(fd)
	 */
}

void	Server::writeToClient(int fd)
{
	Client* client = _clients.getByFd(fd);
	if (!client)
		return;
	const std::string& buf = client->getOutputBuffer();
	if (buf.empty())
		return;
	ssize_t sent = send(fd, buf.c_str(), buf.size(), 0);
	if (sent > 0)
		client->consumeOutput(static_cast<size_t>(sent));
	if (!client->hasPendingOutput() && client->isDisconnectRequested())
		disconnectClient(fd);
}

void	Server::disconnectClient(int fd)
{
	Client* client = _clients.getByFd(fd);
	if (!client)
		return;
	_channels.removeClientFromAllChannels(client);
	_clients.removeClient(fd);
	close(fd);
}