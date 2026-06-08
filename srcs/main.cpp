#include <iostream>
#include <cstdlib>
#include "Server.hpp"
#include <signal.h>
#include <cerrno>
#include "SignalHandler.hpp"

static bool	isValidPort(const std::string& value)
{
	size_t	i;

	if (value.empty())
		return (false);
	i = 0;
	while (i < value.size())
	{
		if (value[i] < '0' || value[i] > '9')
			return (false);
		++i;
	}
	return (true);
}

static bool	parsePort(const std::string& value, int& port)
{
	char*	end;
	long	result;

	if (value.empty())
		return (false);
	errno = 0;
	result = std::strtol(value.c_str(), &end, 10);
	if (errno != 0 || *end != '\0')
		return (false);
	if (result <= 0 || result > 65535)
		return (false);
	port = static_cast<int>(result);
	return (true);
}

int	main(int argc, char** argv)
{
	int	port;

	if (argc != 3)
	{
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}
	if (!isValidPort(argv[1]))
	{
		std::cerr << "Error: invalid port" << std::endl;
		return (1);
	}
	if (!parsePort(argv[1], port))
	{
		std::cerr << "Error: port out of range" << std::endl;
		return (1);
	}
	if (std::string(argv[2]).empty())
	{
		std::cerr << "Error: password cannot be empty" << std::endl;
		return (1);
	}
	try
	{
		setupSignals();
		signal(SIGTSTP, SIG_IGN);
		Server	server(port, argv[2]);
		server.run();
	}
	catch (const std::exception& error)
	{
		std::cerr << "Error: " << error.what() << std::endl;
		return (1);
	}
	return (0);
}