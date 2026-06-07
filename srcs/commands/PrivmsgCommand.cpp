#include "PrivmsgCommand.hpp"
#include "Client.hpp"
#include "ClientManager.hpp"
#include "ChannelManager.hpp"
#include "Channel.hpp"
#include "CommandMessage.hpp"
#include "ReplyBuilder.hpp"
#include "CommandUtils.hpp"
#include <vector>

PrivmsgCommand::PrivmsgCommand() {}
PrivmsgCommand::~PrivmsgCommand() {}

static void sendToChannel(Client& client, ChannelManager& channels,
	const std::string& target, const std::string& text)
{
    Channel* channel = channels.get(target);
    if (!channel)
    {
        client.appendOutput(ReplyBuilder::errNoSuchChannel(client.getNickname(), target));
        return;
    }
    if (!channel->hasClient(&client))
    {
        client.appendOutput(ReplyBuilder::errCannotSendToChan(client.getNickname(), target));
        return;
    }
    channel->broadcastExcept(&client, ReplyBuilder::privmsg(client, target, text));    
}

static void sendToUser(Client& client, ClientManager& clients, const std::string& target, 
    const std::string& text) {
    Client* targetClient = clients.getByNickname(target);

    if (targetClient == NULL)
    {
        client.appendOutput(ReplyBuilder::errNoSuchNick(client.getNickname(), target));
        return;
    }
    targetClient->appendOutput(ReplyBuilder::privmsg(client, target, text));
}

static void	sendToTarget(Client& client, ClientManager& clients,
	ChannelManager& channels, const std::string& target,
	const std::string& text)
{
    if (target.empty())
    {
        client.appendOutput(ReplyBuilder::errNeedMoreParams(client.getNickname(), "PRIVMSG"));
        return;
    }
    if (target[0] == '#') {
        sendToChannel(client, channels, target, text);
    }
    else
    {
        sendToUser(client, clients, target, text);
    }
}

void PrivmsgCommand::execute(Client& client, const CommandMessage& message,
                             ClientManager& clients, ChannelManager& channels)
{
    std::vector<std::string>    targets;
	std::string text;
	size_t  i;

    if (!client.isRegistered())
    {
        client.appendOutput(ReplyBuilder::errNotRegistered(client.getNickname()));
        return;
    }
    if (!message.hasParam(0))
    {
        client.appendOutput(ReplyBuilder::errNeedMoreParams(client.getNickname(), "PRIVMSG"));
        return;
    }
    if (!message.hasTrailing())
    {
        client.appendOutput(ReplyBuilder::errNoTextToSend(client.getNickname()));
        return;
    }
    text = message.getTrailing();
    if (text.empty())
    {
        client.appendOutput(ReplyBuilder::errNoTextToSend(client.getNickname()));
        return;
    }
    targets = CommandUtils::splitComma(message.getParam(0));
	i = 0;
	while (i < targets.size())
	{
		sendToTarget(client, clients, channels, targets[i], text);
		++i;
	}
}