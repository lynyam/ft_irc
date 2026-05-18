#ifndef JOIN_COMMAND_HPP
# define JOIN_COMMAND_HPP

# include "ICommand.hpp"

class JoinCommand : public ICommand
{
    public:
        JoinCommand();
        virtual ~JoinCommand();

        virtual void	execute(Client& client,
                                const CommandMessage& message,
                                ClientManager& clients,
                                ChannelManager& channels);
};

#endif