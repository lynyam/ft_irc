#include "NickCommand.hpp"
#include "Client.hpp"
#include "ClientManager.hpp"
#include "ChannelManager.hpp"
#include "CommandMessage.hpp"
#include "ReplyBuilder.hpp"
#include <cctype>

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
    if (nick.empty())
    {
        client.appendOutput(ReplyBuilder::errNoNicknameGiven());
        return;
    }
    if (nick[0] == '#' || nick[0] == '&' || std::isdigit(static_cast<unsigned char>(nick[0])))
    {
        client.appendOutput(ReplyBuilder::errErroneousNickname(nick));
        return;
    }
    for (std::string::size_type i = 0; i < nick.size(); ++i)
    {
        if (std::isspace(static_cast<unsigned char>(nick[i])) || nick[i] == ',' || nick[i] == ':' || nick[i] == '*')
        {
            client.appendOutput(ReplyBuilder::errErroneousNickname(nick));
            return;
        }
    }
    if (clients.nicknameExists(nick) && nick != client.getNickname())
    {
        client.appendOutput(ReplyBuilder::errNicknameInUse(nick));
        return;
    }
    if (nick == client.getNickname())
        return;

    std::string oldPrefix = client.getPrefix();
    bool wasRegistered = client.isRegistered();
    client.setNickname(nick);
    ReplyBuilder::tryRegister(client);
    if (wasRegistered)
    {
        client.appendOutput(ReplyBuilder::nick(oldPrefix, nick));
        channels.broadcastToClientChannels(&client, ReplyBuilder::nick(oldPrefix, nick));
    }
}