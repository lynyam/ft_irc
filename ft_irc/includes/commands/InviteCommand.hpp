#ifndef INVITE_COMMAND_HPP
# define INVITE_COMMAND_HPP

# include "ICommand.hpp"

class InviteCommand : public ICommand
{
    public:
        InviteCommand();
        virtual ~InviteCommand();

        virtual void	execute(Client& client,
                                const CommandMessage& message,
                                ClientManager& clients,
                                ChannelManager& channels);
};

#endif