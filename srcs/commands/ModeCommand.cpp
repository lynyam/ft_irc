#include "ModeCommand.hpp"
#include "Client.hpp"
#include "ClientManager.hpp"
#include "ChannelManager.hpp"
#include "Channel.hpp"
#include "CommandMessage.hpp"
#include "ReplyBuilder.hpp"
#include <cstdlib>

ModeCommand::ModeCommand() {}
ModeCommand::~ModeCommand() {}

void ModeCommand::execute(Client& client, const CommandMessage& message,
                          ClientManager& clients, ChannelManager& channels)
{
    if (!client.isRegistered())
    {
        client.appendOutput(ReplyBuilder::errNotRegistered(client.getNickname()));
        return;
    }
    if (message.paramCount() < 2)
    {
        client.appendOutput(ReplyBuilder::errNeedMoreParams(client.getNickname(), "MODE"));
        return;
    }
    const std::string& channelName = message.getParam(0);
    Channel* channel = channels.get(channelName);
    if (!channel)
    {
        client.appendOutput(ReplyBuilder::errNoSuchChannel(client.getNickname(), channelName));
        return;
    }
    if (!channel->isOperator(&client))
    {
        client.appendOutput(ReplyBuilder::errChanOPrivsNeeded(client.getNickname(), channelName));
        return;
    }
    bool setting = (message.getParam(1)[0] == '+');
    char mode = message.getParam(1)[1];
    std::string arg = message.paramCount() > 2 ? message.getParam(2) : "";
    if (mode == 'i')
        channel->setInviteOnly(setting);
    else if (mode == 'k')
    {
        if (setting && !arg.empty())
            channel->setKey(arg);
        else
            channel->removeKey();
    }
    else if (mode == 't')
        channel->setTopicProtected(setting);
    else if (mode == 'o' && !arg.empty())
    {
        Client* target = clients.getByNickname(arg);
        if (target == NULL)
        {
            client.appendOutput(ReplyBuilder::errNoSuchNick(client.getNickname(), arg));
            return;
        }
        if (setting)
            channel->addOperator(target);
        else
            channel->removeOperator(target);
    }
    else if (mode == 'l')
    {
        if (setting && !arg.empty())
            channel->setUserLimit(std::atoi(arg.c_str()));
        else
            channel->removeUserLimit();
    }
    channel->broadcast(ReplyBuilder::mode(client, channelName, message.getParam(1), arg));
}