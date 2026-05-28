#ifndef USER_COMMAND_HPP
# define USER_COMMAND_HPP

# include "ICommand.hpp"

class UserCommand : public ICommand
{
    public:
        UserCommand();
        virtual ~UserCommand();

        virtual void	execute(Client& client,
                                const CommandMessage& message,
                                ClientManager& clients,
                                ChannelManager& channels);
};

#endif