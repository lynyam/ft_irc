#ifndef PART_COMMAND_HPP
# define PART_COMMAND_HPP

# include "ICommand.hpp"

class PartCommand : public ICommand
{
    public:
        PartCommand();
        virtual ~PartCommand();

        virtual void	execute(Client& client,
                                const CommandMessage& message,
                                ClientManager& clients,
                                ChannelManager& channels);
};

#endif