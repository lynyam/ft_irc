#include "TopicCommand.hpp"
#include "Client.hpp"
#include "ClientManager.hpp"
#include "ChannelManager.hpp"
#include "Channel.hpp"
#include "CommandMessage.hpp"
#include "ReplyBuilder.hpp"

TopicCommand::TopicCommand() {}
TopicCommand::~TopicCommand() {}

void TopicCommand::execute(Client& client, const CommandMessage& message,
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
        client.appendOutput(ReplyBuilder::errNeedMoreParams(client.getNickname(), "TOPIC"));
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
        client.appendOutput(ReplyBuilder::errNotOnChannel(client.getNickname(), channelName));
        return;
    }
    if (message.paramCount() == 1)
    {
        if (!channel->hasTopic())
        {
            client.appendOutput(":server 331 " + client.getNickname() + " " + channelName + " :No topic is set\r\n");
        }
        else
        {
            client.appendOutput(ReplyBuilder::topic(client.getNickname(), channelName, channel->getTopic()));
        }
        return;
    }
    if (channel->isTopicProtected() && !channel->isOperator(&client))
    {
        client.appendOutput(ReplyBuilder::errChanOPrivsNeeded(client.getNickname(), channelName));
        return;
    }
    const std::string& newTopic = message.getParam(1);
    channel->setTopic(newTopic);
    channel->broadcast(":" + client.getPrefix() + " TOPIC " + channelName + " :" + newTopic + "\r\n");
}