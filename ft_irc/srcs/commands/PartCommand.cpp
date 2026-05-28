#include "PartCommand.hpp"
#include "Client.hpp"
#include "ClientManager.hpp"
#include "ChannelManager.hpp"
#include "Channel.hpp"
#include "CommandMessage.hpp"
#include "ReplyBuilder.hpp"

PartCommand::PartCommand() {}
PartCommand::~PartCommand() {}

void PartCommand::execute(Client& client, const CommandMessage& message,
                          ClientManager& clients, ChannelManager& channels)
{
    (void)clients;
    if (!client.isRegistered())
    {
        client.appendOutput(ReplyBuilder::errNotRegistered(client.getNickname()));
        return;
    }
    if (message.paramCount() < 1)
    {
        client.appendOutput(ReplyBuilder::errNeedMoreParams(client.getNickname(), "PART"));
        return;
    }
    const std::string& channelName = message.getParam(0);
    if (!channels.exists(channelName))
    {
        client.appendOutput(ReplyBuilder::errNoSuchChannel(client.getNickname(), channelName));
        return;
    }
    Channel* channel = channels.get(channelName);
    if (!channel->hasClient(&client))
    {
        client.appendOutput(ReplyBuilder::errNotRegistered(client.getNickname()));
        return;
    }
    std::string reason = message.paramCount() > 1 ? message.getParam(1) : "";
    channel->broadcast(ReplyBuilder::part(client, channelName, reason));
    channel->removeClient(&client);
    channels.removeIfEmpty(channelName);
}