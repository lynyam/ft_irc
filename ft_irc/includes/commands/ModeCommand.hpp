#ifndef MODE_COMMAND_HPP
# define MODE_COMMAND_HPP

# include "ICommand.hpp"

class ModeCommand : public ICommand
{
    public:
        ModeCommand();
        virtual ~ModeCommand();

        virtual void	execute(Client& client,
                                const CommandMessage& message,
                                ClientManager& clients,
                                ChannelManager& channels);
};

#endif