#ifndef PRIVMSG_COMMAND_HPP
# define PRIVMSG_COMMAND_HPP

# include "ICommand.hpp"

class PrivmsgCommand : public ICommand
{
    public:
        PrivmsgCommand();
        virtual ~PrivmsgCommand();

        virtual void	execute(Client& client,
                                const CommandMessage& message,
                                ClientManager& clients,
                                ChannelManager& channels);
};

#endif