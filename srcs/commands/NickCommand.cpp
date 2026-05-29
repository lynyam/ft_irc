#include "NickCommand.hpp"
#include "Client.hpp"
#include "ClientManager.hpp"
#include "ChannelManager.hpp"
#include "CommandMessage.hpp"
#include "ReplyBuilder.hpp"

NickCommand::NickCommand() {}
NickCommand::~NickCommand() {}

void NickCommand::execute(Client& client, const CommandMessage& message,
                          ClientManager& clients, ChannelManager& channels)
{
    (void)channels;
    if (!client.isPasswordAccepted())
    {
        client.appendOutput(ReplyBuilder::errNotRegistered(client.getNickname()));
        return;
    }
    if (message.paramCount() < 1)
    {
        client.appendOutput(ReplyBuilder::errNoNicknameGiven());
        return;
    }
    const std::string& nick = message.getParam(0);
    if (clients.nicknameExists(nick))
    {
        client.appendOutput(ReplyBuilder::errNicknameInUse(nick));
        return;
    }
    client.setNickname(nick);
}