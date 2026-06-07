#include "InviteCommand.hpp"
#include "Client.hpp"
#include "ClientManager.hpp"
#include "ChannelManager.hpp"
#include "Channel.hpp"
#include "CommandMessage.hpp"
#include "ReplyBuilder.hpp"

InviteCommand::InviteCommand() {}
InviteCommand::~InviteCommand() {}

void InviteCommand::execute(Client& client, const CommandMessage& message,
                            ClientManager& clients, ChannelManager& channels)
{
    if (!client.isRegistered())
    {
        client.appendOutput(ReplyBuilder::errNotRegistered(client.getNickname()));
        return;
    }
    if (message.paramCount() < 2)
    {
        client.appendOutput(ReplyBuilder::errNeedMoreParams(client.getNickname(), "INVITE"));
        return;
    }
    const std::string& targetNick = message.getParam(0);
    const std::string& channelName = message.getParam(1);
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
    //FOR IRC-like i will say but to fit the subject i implement only INVITE for Op
    //if (channel->isInviteOnly() && !channel->isOperator(&client))
    if (!channel->isOperator(&client))
    {
        client.appendOutput(ReplyBuilder::errChanOPrivsNeeded(client.getNickname(), channelName));
        return;
    }
    Client* target = clients.getByNickname(targetNick);
    if (target == NULL)
    {
        client.appendOutput(ReplyBuilder::errNoSuchNick(client.getNickname(), targetNick));
        return;
    }
    if (channel->hasClient(target))
    {
        client.appendOutput(ReplyBuilder::errUserOnChannel(client.getNickname(), targetNick, channelName));
        return;
    }
    channel->invite(target);
    client.appendOutput(ReplyBuilder::rplInviting(client.getNickname(), targetNick, channelName));
    target->appendOutput(ReplyBuilder::invite(client, targetNick, channelName));
}