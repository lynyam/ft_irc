#ifndef TOPIC_COMMAND_HPP
# define TOPIC_COMMAND_HPP

# include "ICommand.hpp"

class TopicCommand : public ICommand
{
    public:
        TopicCommand();
        virtual ~TopicCommand();

        virtual void	execute(Client& client,
                                const CommandMessage& message,
                                ClientManager& clients,
                                ChannelManager& channels);
};

#endif