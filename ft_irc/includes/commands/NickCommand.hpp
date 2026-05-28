#ifndef NICK_COMMAND_HPP
# define NICK_COMMAND_HPP

# include "ICommand.hpp"

class NickCommand : public ICommand
{
    public:
        NickCommand();
        virtual ~NickCommand();

        virtual void	execute(Client& client,
                                const CommandMessage& message,
                                ClientManager& clients,
                                ChannelManager& channels);
};

#endif