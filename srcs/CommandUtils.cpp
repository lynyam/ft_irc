#include "CommandUtils.hpp"
#include "Client.hpp"
#include "ReplyBuilder.hpp"

void	CommandUtils::tryRegister(Client& client)
{
	if (!client.isRegistered() && client.canRegister())
	{
		client.markRegistered();
		client.appendOutput(ReplyBuilder::welcome(client.getNickname()));
		client.appendOutput(ReplyBuilder::yourHost(client.getNickname()));
		client.appendOutput(ReplyBuilder::created(client.getNickname()));
	}
}

std::vector<std::string>	CommandUtils::splitComma(const std::string& value)
{
	std::vector<std::string>	result;
	size_t					start;
	size_t					pos;

	start = 0;
	while (start <= value.size())
	{
		pos = value.find(',', start);
		if (pos == std::string::npos)
		{
			result.push_back(value.substr(start));
			break;
		}
		result.push_back(value.substr(start, pos - start));
		start = pos + 1;
	}
	return (result);
}