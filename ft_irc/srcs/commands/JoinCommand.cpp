#include "JoinCommand.hpp"
#include "Client.hpp"
#include "ClientManager.hpp"
#include "ChannelManager.hpp"
#include "Channel.hpp"
#include "CommandMessage.hpp"
#include "ReplyBuilder.hpp"

JoinCommand::JoinCommand() {}
JoinCommand::~JoinCommand() {}

void JoinCommand::execute(Client& client, const CommandMessage& message,
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
        client.appendOutput(ReplyBuilder::errNeedMoreParams(client.getNickname(), "JOIN"));
        return;
    }
    const std::string& channelName = message.getParam(0);
    Channel* channel = channels.getOrCreate(channelName);
    if (channel->isInviteOnly() && !channel->isInvited(&client))
    {
        client.appendOutput(ReplyBuilder::errInviteOnlyChan(client.getNickname(), channelName));
        channels.removeIfEmpty(channelName);
        return;
    }
    if (channel->hasKey() && (message.paramCount() < 2 || !channel->checkKey(message.getParam(1))))
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
    channel->broadcast(ReplyBuilder::join(client, channelName));
    client.appendOutput(ReplyBuilder::namReply(client.getNickname(), channelName, channel->buildNamesList()));
    client.appendOutput(ReplyBuilder::endOfNames(client.getNickname(), channelName));
}