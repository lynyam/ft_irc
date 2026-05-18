#ifndef QUIT_COMMAND_HPP
# define QUIT_COMMAND_HPP

# include "ICommand.hpp"

class QuitCommand : public ICommand
{
    public:
        QuitCommand();
        virtual ~QuitCommand();

        virtual void	execute(Client& client,
                                const CommandMessage& message,
                                ClientManager& clients,
                                ChannelManager& channels);
};

#endif