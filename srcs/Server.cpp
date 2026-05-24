#include "Server.hpp"
#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

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
	initSocket();
	_running = true;
	std::cout << "ircserv listening on port " << _port << std::endl;
	/* TODO(Leon): */
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
	int	option;
	struct sockaddr_in	address;

	 _serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverFd < 0) {
		throw std::runtime_error("socket failed");
	}
	option = 1;
	if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR,
		&option, sizeof(option))) {
			throw std::runtime_error("setsocketopt failed");
	}
	//clean the structure by fill octect with zero
	std::memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	//convert host short to host byte -> network byte
	address.sin_port = htons(_port);
	//convert host long to host byte -> network byte
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(_serverFd, reinterpret_cast<struct sockaddr*>(&address),
		sizeof(address)) < 0) {
		throw std::runtime_error("bind failed");
	}
	if (listen(_serverFd, SOMAXCONN) < 0) {
		throw std::runtime_error("listen failed");
	}
	setNonBlocking(_serverFd);

	std::cout << "_serverFd is " << _serverFd << std::endl;
	sleep(100);
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
	(void)fd;
    /* TODO(Leon):
	 * - recv()
	 * - if n > 0: appendInput()
	 * - while hasCompleteLine(): popLine() and dispatcher.dispatch()
	 * - if n == 0: disconnectClient()
	 */
}

void	Server::writeToClient(int fd)
{
	(void)fd;
    /* TODO(Leon):
	 * - get Client*
	 * - send output buffer
	 * - consume sent bytes
	 */
}

void	Server::disconnectClient(int fd)
{
	(void)fd;
    /* TODO(Leon):
	 * - get Client*
	 * - _channels.removeClientFromAllChannels(client)
	 * - _clients.removeClient(fd)
	 * - close(fd)
	 */
}