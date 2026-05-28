#ifndef KICK_COMMAND_HPP
# define KICK_COMMAND_HPP

# include "ICommand.hpp"

class KickCommand : public ICommand
{
    public:
        KickCommand();
        virtual ~KickCommand();

        virtual void	execute(Client& client,
                                const CommandMessage& message,
                                ClientManager& clients,
                                ChannelManager& channels);
};

#endif