#include "PrivmsgCommand.hpp"
#include "Client.hpp"
#include "ClientManager.hpp"
#include "ChannelManager.hpp"
#include "Channel.hpp"
#include "CommandMessage.hpp"
#include "ReplyBuilder.hpp"

PrivmsgCommand::PrivmsgCommand() {}
PrivmsgCommand::~PrivmsgCommand() {}

void PrivmsgCommand::execute(Client& client, const CommandMessage& message,
                             ClientManager& clients, ChannelManager& channels)
{
    if (!client.isRegistered())
    {
        client.appendOutput(ReplyBuilder::errNotRegistered(client.getNickname()));
        return;
    }
    if (message.paramCount() < 2)
    {
        client.appendOutput(ReplyBuilder::errNeedMoreParams(client.getNickname(), "PRIVMSG"));
        return;
    }
    const std::string& target = message.getParam(0);
    const std::string& text = message.getParam(1);
    if (!target.empty() && target[0] == '#')
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
    else
    {
        Client* targetClient = clients.getByNickname(target);
        if (targetClient == NULL)
        {
            client.appendOutput(ReplyBuilder::errNoSuchNick(client.getNickname(), target));
            return;
        }
        targetClient->appendOutput(ReplyBuilder::privmsg(client, target, text));
    }
}