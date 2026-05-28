#include "UserCommand.hpp"
#include "Client.hpp"
#include "ClientManager.hpp"
#include "ChannelManager.hpp"
#include "CommandMessage.hpp"
#include "ReplyBuilder.hpp"

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
    if (message.paramCount() < 4)
    {
        client.appendOutput(ReplyBuilder::errNeedMoreParams(client.getNickname(), "USER"));
        return;
    }
    client.setUsername(message.getParam(0));
    client.setRealname(message.getParam(3));
    if (client.canRegister())
    {
        client.markRegistered();
        client.appendOutput(ReplyBuilder::welcome(client.getNickname()));
        client.appendOutput(ReplyBuilder::yourHost(client.getNickname()));
        client.appendOutput(ReplyBuilder::created(client.getNickname()));
    }
}