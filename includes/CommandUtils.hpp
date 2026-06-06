#ifndef COMMAND_UTILS_HPP
# define COMMAND_UTILS_HPP

# include <string>
# include <vector>

class Client;

namespace CommandUtils
{
	void	tryRegister(Client& client);
	std::vector<std::string>	splitComma(const std::string& value);
}

#endif