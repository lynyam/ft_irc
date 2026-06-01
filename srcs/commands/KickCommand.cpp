#include "KickCommand.hpp"
#include "Client.hpp"
#include "ClientManager.hpp"
#include "ChannelManager.hpp"
#include "Channel.hpp"
#include "CommandMessage.hpp"
#include "ReplyBuilder.hpp"

KickCommand::KickCommand() {}
KickCommand::~KickCommand() {}

void KickCommand::execute(Client& client, const CommandMessage& message,
                          ClientManager& clients, ChannelManager& channels)
{
    if (!client.isRegistered())
    {
        client.appendOutput(ReplyBuilder::errNotRegistered(client.getNickname()));
        return;
    }
    if (message.paramCount() < 2)
    {
        client.appendOutput(ReplyBuilder::errNeedMoreParams(client.getNickname(), "KICK"));
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
    if (!channel->isOperator(&client))
    {
        client.appendOutput(ReplyBuilder::errChanOPrivsNeeded(client.getNickname(), channelName));
        return;
    }

    const std::string& targetNick = message.getParam(1);
    Client* target = clients.getByNickname(targetNick);
    if (target == NULL)
    {
        client.appendOutput(ReplyBuilder::errNoSuchNick(client.getNickname(), targetNick));
        return;
    }
    if (target == &client)
    {
        client.appendOutput(":server NOTICE " + client.getNickname() + " :Operator cannot kick themselves\r\n");
        return;
    }
    if (!channel->hasClient(target))
    {
        client.appendOutput(ReplyBuilder::errUserNotInChannel(client.getNickname(), targetNick, channelName));
        return;
    }

    std::string reason = message.paramCount() > 2 ? message.getParam(2) : targetNick;
    std::string kickMsg = ":" + client.getPrefix() + " KICK " + channelName + " " + targetNick + " :" + reason + "\r\n";
    channel->broadcast(kickMsg);
    channel->removeClient(target);
}