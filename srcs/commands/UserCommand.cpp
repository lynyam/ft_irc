#include "UserCommand.hpp"
#include "Client.hpp"
#include "ClientManager.hpp"
#include "ChannelManager.hpp"
#include "CommandMessage.hpp"
#include "ReplyBuilder.hpp"
#include "CommandUtils.hpp"

UserCommand::UserCommand() {}
UserCommand::~UserCommand() {}

void UserCommand::execute(Client& client, const CommandMessage& message,
                          ClientManager& clients, ChannelManager& channels)
{
    (void)clients;
    (void)channels;
    if (!client.isPasswordAccepted())
    {
        client.appendOutput(ReplyBuilder::errNotRegistered(client.getNickname()));
        return;
    }
    if (client.isRegistered())
    {
        client.appendOutput(ReplyBuilder::errAlreadyRegistered(client.getNickname()));
        return;
    }
    if (!message.hasParam(0) || !message.hasParam(1) || !message.hasParam(2))
    {
        client.appendOutput(ReplyBuilder::errNeedMoreParams(client.getNickname(), "USER"));
        return;
    }
    std::string userName = message.getParam(0);
    client.setUsername(userName);
    std::string realName = message.getTrailing();
    if (!message.hasTrailing() || realName.empty())
        realName = userName;
    client.setRealname(realName);
    CommandUtils::tryRegister(client);
}