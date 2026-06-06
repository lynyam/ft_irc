#include "PartCommand.hpp"
#include "Client.hpp"
#include "ClientManager.hpp"
#include "ChannelManager.hpp"
#include "Channel.hpp"
#include "CommandMessage.hpp"
#include "ReplyBuilder.hpp"
#include "CommandUtils.hpp"

PartCommand::PartCommand() {}
PartCommand::~PartCommand() {}

static void	partOneChannel(Client& client, ChannelManager& channels,
	const std::string& channelName, const std::string& reason)
{
	Channel*	channel;
	bool		wasOperator;

	channel = channels.get(channelName);
	if (!channel)
	{
		client.appendOutput(ReplyBuilder::errNoSuchChannel(
			client.getNickname(), channelName));
		return;
	}
	if (!channel->hasClient(&client))
	{
		client.appendOutput(ReplyBuilder::errNotOnChannel(
			client.getNickname(), channelName));
		return;
	}
	wasOperator = channel->isOperator(&client);
	channel->broadcast(ReplyBuilder::part(client, channelName, reason));
	channel->removeClient(&client);
	channels.removeIfEmpty(channelName);
	if (!channels.exists(channelName))
		return;
    //auto-Prom
	if (wasOperator && !channel->hasOperator())
	{
		Client* newOp;

		newOp = channel->getFirstMember();
		if (newOp)
		{
			channel->addOperator(newOp);
			channel->broadcast(ReplyBuilder::mode(
				"server", channelName, "+o", newOp->getNickname()));
		}
	}
}

void PartCommand::execute(Client& client, const CommandMessage& message,
                          ClientManager& clients, ChannelManager& channels)
{
    std::vector<std::string>    channelNames;
	std::string reason;
	size_t  i;

    (void)clients;
    if (!client.isRegistered())
    {
        client.appendOutput(ReplyBuilder::errNotRegistered(client.getNickname()));
        return;
    }
    if (!message.hasParam(0))
    {
        client.appendOutput(ReplyBuilder::errNeedMoreParams(client.getNickname(), "PART"));
        return;
    }
    reason = message.getTrailing();
	if (reason.empty() && message.hasParam(1))
		reason = message.getParam(1);
	channelNames = CommandUtils::splitComma(message.getParam(0));
	i = 0;
	while (i < channelNames.size())
	{
		partOneChannel(client, channels, channelNames[i], reason);
		++i;
	}
}