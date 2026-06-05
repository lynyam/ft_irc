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