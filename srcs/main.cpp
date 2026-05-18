#include <iostream>
#include <cstdlib>
#include "Server.hpp"

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
	port = std::atoi(argv[1]);
	if (port <= 0 || port > 65535)
	{
		std::cerr << "Error: port out of range" << std::endl;
		return (1);
	}
	try
	{
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