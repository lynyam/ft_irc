#include "CommandDispatcher.hpp"
#include "Parser.hpp"
#include "CommandMessage.hpp"
#include "Client.hpp"

CommandDispatcher::CommandDispatcher(ClientManager& clients,
									 ChannelManager& channels,
									 const std::string& password)
	: _commands(),
	  _clients(clients),
	  _channels(channels),
	  _password(password)
{
	registerCommands();
}

CommandDispatcher::~CommandDispatcher()
{
	CommandMap::iterator	it;

	it = _commands.begin();
	while (it != _commands.end())
	{
		delete it->second;
		++it;
	}
	_commands.clear();
}

void	CommandDispatcher::dispatch(Client& client, const std::string& rawLine)
{
	CommandMessage	message;

	message = Parser::parse(rawLine);
	dispatch(client, message);
}

void	CommandDispatcher::dispatch(Client& client,
	const CommandMessage& message)
{
	CommandMap::iterator	it;

	it = _commands.find(message.getCommand());
	if (it == _commands.end())
	{
		(void)client;
		return ;
	}
	it->second->execute(client, message, _clients, _channels);
}

void	CommandDispatcher::registerCommands()
{
	/*
	 * TODO(Yurong):
	 * Register command objects here.
	 *
	 * Example later:
	 * _commands["PASS"] = new PassCommand(_password);
	 * _commands["NICK"] = new NickCommand();
	 * _commands["USER"] = new UserCommand();
	 * _commands["JOIN"] = new JoinCommand();
	 * _commands["PRIVMSG"] = new PrivmsgCommand();
	 *
	 * For now, keep it empty so the project compiles.
	 */
}