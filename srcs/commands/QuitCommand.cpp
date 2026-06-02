#include "QuitCommand.hpp"
#include "Client.hpp"
#include "ClientManager.hpp"
#include "ChannelManager.hpp"
#include "CommandMessage.hpp"
#include "ReplyBuilder.hpp"

QuitCommand::QuitCommand() {}
QuitCommand::~QuitCommand() {}

void QuitCommand::execute(Client& client, const CommandMessage& message,
                          ClientManager& clients, ChannelManager& channels)
{
    (void)clients;
    std::string reason = message.getTrailing();
    if (reason.empty() && message.hasParam(0))
        reason = message.getParam(0);
    if (reason.empty())
        reason = "Client Quit";
    channels.broadcastToClientChannels(&client, ReplyBuilder::quit(client, reason));
    channels.removeClientFromAllChannels(&client);
    client.appendOutput(ReplyBuilder::errorClosingLink(client.getNickname(), reason));
    client.requestDisconnect();
}