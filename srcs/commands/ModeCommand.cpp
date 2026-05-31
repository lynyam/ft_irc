#include "ModeCommand.hpp"
#include "Client.hpp"
#include "ClientManager.hpp"
#include "ChannelManager.hpp"
#include "Channel.hpp"
#include "CommandMessage.hpp"
#include "ReplyBuilder.hpp"
#include <cstdlib>
#include <cctype>

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
    const std::string& modeStr = message.getParam(1);
    if (modeStr.size() < 2 || (modeStr[0] != '+' && modeStr[0] != '-'))
    {
        client.appendOutput(ReplyBuilder::errNeedMoreParams(client.getNickname(), "MODE"));
        return;
    }

    bool setting = (modeStr[0] == '+');
    char mode = modeStr[1];
    std::string arg = message.paramCount() > 2 ? message.getParam(2) : "";

    if (mode == 'i')
        channel->setInviteOnly(setting);
    else if (mode == 't')
        channel->setTopicProtected(setting);
    else if (mode == 'k')
    {
        if (setting)
        {
            if (arg.empty())
            {
                client.appendOutput(ReplyBuilder::errNeedMoreParams(client.getNickname(), "MODE"));
                return;
            }
            channel->setKey(arg);
        }
        else
            channel->removeKey();
    }
    else if (mode == 'l')
    {
        if (setting)
        {
            if (arg.empty())
            {
                client.appendOutput(ReplyBuilder::errNeedMoreParams(client.getNickname(), "MODE"));
                return;
            }
            for (std::string::size_type i = 0; i < arg.size(); ++i)
            {
                if (!std::isdigit(static_cast<unsigned char>(arg[i])))
                {
                    client.appendOutput(ReplyBuilder::errNeedMoreParams(client.getNickname(), "MODE"));
                    return;
                }
            }
            int limit = std::atoi(arg.c_str());
            if (limit <= 0)
            {
                client.appendOutput(ReplyBuilder::errNeedMoreParams(client.getNickname(), "MODE"));
                return;
            }
            channel->setUserLimit(static_cast<size_t>(limit));
        }
        else
            channel->removeUserLimit();
    }
    else if (mode == 'o')
    {
        if (arg.empty())
        {
            client.appendOutput(ReplyBuilder::errNeedMoreParams(client.getNickname(), "MODE"));
            return;
        }
        Client* target = clients.getByNickname(arg);
        if (target == NULL)
        {
            client.appendOutput(ReplyBuilder::errNoSuchNick(client.getNickname(), arg));
            return;
        }
        if (!channel->hasClient(target))
        {
            client.appendOutput(ReplyBuilder::errUserNotInChannel(client.getNickname(), arg, channelName));
            return;
        }
        if (setting)
            channel->addOperator(target);
        else
            channel->removeOperator(target);
    }
    else
        return;

    channel->broadcast(ReplyBuilder::mode(client, channelName, modeStr, arg));
}