#ifndef ICOMMAND_HPP
# define ICOMMAND_HPP

class Client;
class ClientManager;
class ChannelManager;
class CommandMessage;

class ICommand
{
    public:
        virtual ~ICommand() {}

        virtual void	execute(Client& client,
                                const CommandMessage& message,
                                ClientManager& clients,
                                ChannelManager& channels) = 0;
};

#endif