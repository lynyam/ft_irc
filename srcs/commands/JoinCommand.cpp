#include "JoinCommand.hpp"
#include "Client.hpp"
#include "ClientManager.hpp"
#include "ChannelManager.hpp"
#include "Channel.hpp"
#include "CommandMessage.hpp"
#include "ReplyBuilder.hpp"
#include "CommandUtils.hpp"

JoinCommand::JoinCommand() {}
JoinCommand::~JoinCommand() {}



static void	joinOneChannel(Client& client, ChannelManager& channels,
	const std::string& channelName, const std::string& key)
{
    bool    isNewChannel;
    Channel*    channel;

    if (channelName.empty() || channelName[0] != '#')
    {
        client.appendOutput(ReplyBuilder::errNoSuchChannel(client.getNickname(), channelName));
        return;
    }
    isNewChannel = !channels.exists(channelName);
    channel = channels.getOrCreate(channelName);
    if (!channel)
    {
        client.appendOutput(ReplyBuilder::errNoSuchChannel(client.getNickname(), channelName));
        return;
    }
    if (channel->hasClient(&client))
    {
        client.appendOutput(ReplyBuilder::namReply(client.getNickname(), channelName, channel->buildNamesList()));
        client.appendOutput(ReplyBuilder::endOfNames(client.getNickname(), channelName));
        return;
    }
    if (channel->isInviteOnly() && !channel->isInvited(&client))
    {
        client.appendOutput(ReplyBuilder::errInviteOnlyChan(client.getNickname(), channelName));
        channels.removeIfEmpty(channelName);
        return;
    }
    if (channel->hasKey() && !channel->checkKey(key))
    {
        client.appendOutput(ReplyBuilder::errBadChannelKey(client.getNickname(), channelName));
        channels.removeIfEmpty(channelName);
        return;
    }
    if (channel->isFull())
    {
        client.appendOutput(ReplyBuilder::errChannelIsFull(client.getNickname(), channelName));
        channels.removeIfEmpty(channelName);
        return;
    }
    channel->addClient(&client);
    if (channel->isInvited(&client))
        channel->removeInvite(&client);
    if (isNewChannel)
        channel->addOperator(&client);
    channel->broadcast(ReplyBuilder::join(client, channelName));
    if (channel->hasTopic()) {
        client.appendOutput(ReplyBuilder::topic( client.getNickname(), channelName, 
            channel->getTopic()));
    }
    client.appendOutput(ReplyBuilder::namReply(client.getNickname(), channelName, channel->buildNamesList()));
    client.appendOutput(ReplyBuilder::endOfNames(client.getNickname(), channelName));
}

void JoinCommand::execute(Client& client, const CommandMessage& message,
                          ClientManager& clients, ChannelManager& channels)
{
    (void)clients;
    std::vector<std::string>	channelNames;
	std::vector<std::string>	keys;
	size_t  i;
	std::string key;

    if (!client.isRegistered())
    {
        client.appendOutput(ReplyBuilder::errNotRegistered(client.getNickname()));
        return;
    }
    if (!message.hasParam(0))
    {
        client.appendOutput(ReplyBuilder::errNeedMoreParams(client.getNickname(), "JOIN"));
        return;
    }
    channelNames = CommandUtils::splitComma(message.getParam(0));
    if (message.hasParam(1)) {
		keys = CommandUtils::splitComma(message.getParam(1));
    }
    i = 0;
	while (i < channelNames.size())
	{
		key = "";
		if (i < keys.size())
			key = keys[i];
		joinOneChannel(client, channels, channelNames[i], key);
		++i;
	}
}