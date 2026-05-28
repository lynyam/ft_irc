#ifndef COMMAND_DISPATCHER_HPP
# define COMMAND_DISPATCHER_HPP

# include <map>
# include <string>
# include "ICommand.hpp"
# include "ClientManager.hpp"
# include "ChannelManager.hpp"

class CommandMessage;

class CommandDispatcher
{
    public:
        CommandDispatcher(ClientManager& clients,
                        ChannelManager& channels,
                        const std::string& password);
        ~CommandDispatcher();

        void	dispatch(Client& client, const std::string& rawLine);
        void	dispatch(Client& client, const CommandMessage& message);

    private:
        typedef std::map<std::string, ICommand*>	CommandMap;

        CommandMap			_commands;
        ClientManager&		_clients;
        ChannelManager&		_channels;
        std::string			_password;

    private:
        void	registerCommands();
        CommandDispatcher();
        CommandDispatcher(const CommandDispatcher& other);
        CommandDispatcher&	operator=(const CommandDispatcher& other);
};

#endif