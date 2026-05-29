#include "QuitCommand.hpp"
#include "Client.hpp"
#include "ClientManager.hpp"
#include "ChannelManager.hpp"
#include "CommandMessage.hpp"

QuitCommand::QuitCommand() {}
QuitCommand::~QuitCommand() {}

void QuitCommand::execute(Client& client, const CommandMessage& message,
                          ClientManager& clients, ChannelManager& channels)
{
    std::string reason = message.paramCount() > 0 ? message.getParam(0) : "Client Quit";
    std::string quitMsg = ":" + client.getPrefix() + " QUIT :" + reason + "\r\n";
    channels.removeClientFromAllChannels(&client);
    client.appendOutput("ERROR :Closing Link: " + client.getNickname() + " (" + reason + ")\r\n");
    clients.removeClient(client.getFd());
}