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
    std::string token;
    if (message.hasTrailing())
        token = message.getTrailing();
    else if (message.hasParam(0))
        token = message.getParam(0);
    if (token.empty())
    {
        client.appendOutput(ReplyBuilder::errNeedMoreParams(client.getNickname(), "PING"));
        return;
    }
    client.appendOutput(ReplyBuilder::pong(token));
}
