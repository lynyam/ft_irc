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
    if (!message.hasParam(0))
    {
        client.appendOutput(ReplyBuilder::errNeedMoreParams(client.getNickname(), "PART"));
        return;
    }
    const std::string& channelName = message.getParam(0);
    Channel* channel = channels.get(channelName);
    if (!channel)
    {
        client.appendOutput(ReplyBuilder::errNoSuchChannel(client.getNickname(), channelName));
        return;
    }
    if (!channel->hasClient(&client))
    {
        client.appendOutput(ReplyBuilder::errNotOnChannel(client.getNickname(), channelName));
        return;
    }

    //check operator status before removing
    bool wasOperator = channel->isOperator(&client);

    std::string reason = message.getTrailing();
    if (reason.empty() && message.hasParam(1))
        reason = message.getParam(1);
    channel->broadcast(ReplyBuilder::part(client, channelName, reason));
    channel->removeClient(&client);
    channels.removeIfEmpty(channelName);
    if (!channels.exists(channelName))
        return ;

    //auto-promote if operator left and no operator remains
    if (wasOperator && !channel->hasOperator())
    {
        Client* newOp = channel->getFirstMember();
        if (newOp)
        {
            channel->addOperator(newOp);
            channel->broadcast(ReplyBuilder::mode("server", channelName, "+o", newOp->getNickname()));
        }
    }
}