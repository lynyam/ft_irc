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
    std::string reason = message.getTrailing().empty() ? "Client Quit" : message.getTrailing();
    channels.broadcastToClientChannels(&client, ReplyBuilder::quit(client, reason));
    channels.removeClientFromAllChannels(&client);
    client.appendOutput("ERROR :Closing Link: " + client.getNickname() + " (" + reason + ")\r\n");
    client.markPendingDisconnect();
}