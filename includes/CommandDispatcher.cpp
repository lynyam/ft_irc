#include "CommandDispatcher.hpp"
#include "CommandMessage.hpp"
#include "Client.hpp"
#include "Parser.hpp"
#include "PassCommand.hpp"
#include "NickCommand.hpp"
#include "UserCommand.hpp"
#include "JoinCommand.hpp"
#include "PartCommand.hpp"
#include "PrivmsgCommand.hpp"
#include "InviteCommand.hpp"
#include "QuitCommand.hpp"
#include "KickCommand.hpp"
#include "TopicCommand.hpp"
#include "ModeCommand.hpp"

CommandDispatcher::CommandDispatcher(ClientManager& clients,
                                     ChannelManager& channels,
                                     const std::string& password)
    : _clients(clients), _channels(channels), _password(password)
{
    registerCommands();
}

CommandDispatcher::~CommandDispatcher()
{
    for (CommandMap::iterator it = _commands.begin(); it != _commands.end(); ++it)
        delete it->second;
}

void CommandDispatcher::registerCommands()
{
    _commands["PASS"]    = new PassCommand(_password);
    _commands["NICK"]    = new NickCommand();
    _commands["USER"]    = new UserCommand();
    _commands["JOIN"]    = new JoinCommand();
    _commands["PART"]    = new PartCommand();
    _commands["PRIVMSG"] = new PrivmsgCommand();
    _commands["INVITE"]  = new InviteCommand();
    _commands["QUIT"]    = new QuitCommand();
    _commands["KICK"]    = new KickCommand();
    _commands["TOPIC"]   = new TopicCommand();
    _commands["MODE"]    = new ModeCommand();
}

void CommandDispatcher::dispatch(Client& client, const CommandMessage& message)
{
    CommandMap::iterator it = _commands.find(message.getCommand());
    if (it == _commands.end())
    {
        client.appendOutput(":server 421 " + client.getNickname() + " " + message.getCommand() + " :Unknown command\r\n");
        return;
    }
    it->second->execute(client, message, _clients, _channels);
}

void CommandDispatcher::dispatch(Client& client, const std::string& rawLine)
{
    CommandMessage cmdMsg = Parser::parse(rawLine);
    dispatch(client, cmdMsg);
}