#ifndef PING_COMMAND_HPP
# define PING_COMMAND_HPP

# include "ICommand.hpp"

class PingCommand : public ICommand
{
    public:
        PingCommand();
        ~PingCommand();

        void execute(Client& client, const CommandMessage& message,
                     ClientManager& clients, ChannelManager& channels);
};

#endif
