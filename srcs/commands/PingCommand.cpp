#include "PingCommand.hpp"
#include "Client.hpp"
#include "ClientManager.hpp"
#include "ChannelManager.hpp"
#include "CommandMessage.hpp"
#include "ReplyBuilder.hpp"

PingCommand::PingCommand() {}
PingCommand::~PingCommand() {}

void PingCommand::execute(Client& client, const CommandMessage& message,
                          ClientManager& clients, ChannelManager& channels)
{
    (void)clients;
    (void)channels;
    std::string token = message.getTrailing().empty()
        ? message.getParam(0)
        : message.getTrailing();
    client.appendOutput(ReplyBuilder::pong(token));
}
