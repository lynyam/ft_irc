#include "PassCommand.hpp"
#include "Client.hpp"
#include "ReplyBuilder.hpp"
#include "CommandMessage.hpp"

PassCommand::PassCommand(const std::string& password)
    : _serverPassword(password) {}

/**
 * check if parameters are empty
 * check if the password is correct
 */
void PassCommand::execute(Client& client, const CommandMessage& message,
                          ClientManager& clients, ChannelManager& channels)
{
    (void)clients;
    (void)channels;
    if (client.isRegistered())
    {
        client.appendOutput(ReplyBuilder::errAlreadyRegistered(client.getNickname()));
        return;
    }
    if (!message.hasParam(0))
    {
        client.appendOutput(ReplyBuilder::errNeedMoreParams(client.getNickname(), "PASS"));
        return;
    }
    if (message.getParam(0) != _serverPassword)
    {
        client.appendOutput(ReplyBuilder::errPasswordMismatch());
        return;
    }
    client.setPasswordAccepted(true);
}

PassCommand::~PassCommand() {}