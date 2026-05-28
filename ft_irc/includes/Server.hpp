#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <sys/select.h>
# include "ClientManager.hpp"
# include "ChannelManager.hpp"
# include "CommandDispatcher.hpp"

class Server
{
    public:
        Server(int port, const std::string& password);
        ~Server();

        void	run();
        void	stop();

    private:
        int					_port;
        std::string			_password;
        int					_serverFd;
        bool				_running;

        ClientManager		_clients;
        ChannelManager		_channels;
        CommandDispatcher	_dispatcher;

    private:
        void	initSocket();
        void	setNonBlocking(int fd);

        void	eventLoop();
        void	prepareReadSet(fd_set& readSet, int& maxFd);
        void	prepareWriteSet(fd_set& writeSet, int& maxFd);

        void	handleReadableFds(fd_set& readSet, int maxFd);
        void	handleWritableFds(fd_set& writeSet, int maxFd);

        void	acceptClient();
        void	readFromClient(int fd);
        void	writeToClient(int fd);
        void	disconnectClient(int fd);

    private:
        Server();
        Server(const Server& other);
        Server&	operator=(const Server& other);
};

#endif