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
    if (nick.empty() || nick[0] == '#' || nick[0] == '&' || std::isdigit(static_cast<unsigned char>(nick[0])))
    {
        client.appendOutput(ReplyBuilder::errNoNicknameGiven());
        return;
    }
    for (std::string::size_type i = 0; i < nick.size(); ++i)
    {
        if (std::isspace(static_cast<unsigned char>(nick[i])) || nick[i] == ',' || nick[i] == ':' || nick[i] == '*')
        {
            client.appendOutput(ReplyBuilder::errNoNicknameGiven());
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
    if (!wasRegistered && client.isRegistered())
    {
        client.appendOutput(ReplyBuilder::welcome(client.getNickname()));
        client.appendOutput(ReplyBuilder::yourHost(client.getNickname()));
        client.appendOutput(ReplyBuilder::created(client.getNickname()));
    }
    else if (wasRegistered)
        client.appendOutput(":" + oldPrefix + " NICK :" + nick + "\r\n");
}