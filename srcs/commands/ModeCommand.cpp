#include "ModeCommand.hpp"
#include "Client.hpp"
#include "ClientManager.hpp"
#include "ChannelManager.hpp"
#include "Channel.hpp"
#include "CommandMessage.hpp"
#include "ReplyBuilder.hpp"
#include <cstdlib>
#include <cerrno>
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
    if (!message.hasParam(0))
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
    if (!message.hasParam(1))
    {
        client.appendOutput(ReplyBuilder::mode("server", channelName, channel->buildModeString(), ""));
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
    if (modeStr.size() != 2 || (modeStr[0] != '+' && modeStr[0] != '-'))
    {
        client.appendOutput(ReplyBuilder::errNeedMoreParams(client.getNickname(), "MODE"));
        return;
    }

    bool setting = (modeStr[0] == '+');
    char mode = modeStr[1];
    std::string arg = message.hasParam(2) ? message.getParam(2) : "";

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
            errno = 0;
            char* end;
            long limit = std::strtol(arg.c_str(), &end, 10);
            if (errno != 0 || end == arg.c_str() || limit <= 0 || limit > 65535)
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
        {
            if (!channel->isOperator(target))
                return;
            if (channel->getOperatorCount() == 1)
            {
                if (channel->getClientCount() == 1)
                {
                    channel->broadcast(ReplyBuilder::mode(client, channelName, "-o", target->getNickname()));
                    channels.remove(channelName);
                    return;
                }
                Client* newOp = channel->getFirstMemberExcept(target);
                if (newOp)
                {
                    channel->removeOperator(target);
                    channel->addOperator(newOp);
                    channel->broadcast(ReplyBuilder::mode(client, channelName, "-o", target->getNickname()));
                    channel->broadcast(ReplyBuilder::mode("server", channelName, "+o", newOp->getNickname()));
                    return;
                }
            }
            channel->removeOperator(target);
        }
    }
    else
    {
        client.appendOutput(ReplyBuilder::errUnknownMode(client.getNickname(), mode));
        return;
    }
    if (mode == 'i' || mode == 't' || ((mode == 'k' || mode == 'l') && !setting)) {
        arg = "";
    }
    channel->broadcast(ReplyBuilder::mode(client, channelName, modeStr, arg));
}