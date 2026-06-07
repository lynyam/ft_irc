#include "KickCommand.hpp"
#include "Client.hpp"
#include "ClientManager.hpp"
#include "ChannelManager.hpp"
#include "Channel.hpp"
#include "CommandMessage.hpp"
#include "ReplyBuilder.hpp"
#include "CommandUtils.hpp"

KickCommand::KickCommand() {}
KickCommand::~KickCommand() {}

static void	promoteIfNeeded(Channel* channel, const std::string& channelName)
{
	Client*	newOp;

	if (!channel || channel->hasOperator())
		return;
	newOp = channel->getFirstMember();
	if (newOp)
	{
		channel->addOperator(newOp);
		channel->broadcast(ReplyBuilder::mode(
			"server", channelName, "+o", newOp->getNickname()));
	}
}

static void	kickOneTarget(Client& client, ClientManager& clients,
	ChannelManager& channels, const std::string& channelName,
	const std::string& targetNick, const std::string& reason)
{
	Channel*	channel;
	Client*		target;
	bool		targetWasOperator;

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
	if (!channel->isOperator(&client))
	{
		client.appendOutput(ReplyBuilder::errChanOPrivsNeeded(
			client.getNickname(), channelName));
		return;
	}
	if (targetNick.empty())
	{
		client.appendOutput(ReplyBuilder::errNeedMoreParams(
			client.getNickname(), "KICK"));
		return;
	}
	target = clients.getByNickname(targetNick);
	if (target == NULL)
	{
		client.appendOutput(ReplyBuilder::errNoSuchNick(
			client.getNickname(), targetNick));
		return;
	}
	if (!channel->hasClient(target))
	{
		client.appendOutput(ReplyBuilder::errUserNotInChannel(
			client.getNickname(), targetNick, channelName));
		return;
	}
	targetWasOperator = channel->isOperator(target);
	channel->broadcast(ReplyBuilder::kick(
		client, channelName, targetNick, reason));
	channel->removeClient(target);
	channels.removeIfEmpty(channelName);
	channel = channels.get(channelName);
	if (!channel)
		return;
	if (targetWasOperator)
		promoteIfNeeded(channel, channelName);
}

static std::string	getKickReason(const CommandMessage& message)
{
	std::string	reason;

	reason = message.getTrailing();
	if (reason.empty() && message.hasParam(2))
		reason = message.getParam(2);
	return (reason);
}

void KickCommand::execute(Client& client, const CommandMessage& message,
                          ClientManager& clients, ChannelManager& channels)
{
    std::vector<std::string>	channelNames;
	std::vector<std::string>	targetNames;
	std::string     reason;
	size_t          i;

    if (!client.isRegistered())
    {
        client.appendOutput(ReplyBuilder::errNotRegistered(client.getNickname()));
        return;
    }
    if (!message.hasParam(0) || !message.hasParam(1))
    {
        client.appendOutput(ReplyBuilder::errNeedMoreParams(client.getNickname(), "KICK"));
        return;
    }
    channelNames = CommandUtils::splitComma(message.getParam(0));
	targetNames = CommandUtils::splitComma(message.getParam(1));
	reason = getKickReason(message);
    i = 0;
    if (channelNames.size() == 1)
	{
		while (i < targetNames.size())
		{
			kickOneTarget(client, clients, channels, channelNames[0],
				targetNames[i], reason);
			++i;
		}
		return;
	}
	if (channelNames.size() != targetNames.size())
	{
		client.appendOutput(ReplyBuilder::errNeedMoreParams(
			client.getNickname(), "KICK"));
		return;
	}
	while (i < channelNames.size())
	{
		kickOneTarget(client, clients, channels, channelNames[i],
			targetNames[i], reason);
		++i;
	}
}