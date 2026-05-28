#ifndef PASS_COMMAND_HPP
# define PASS_COMMAND_HPP

# include <string>
# include "ICommand.hpp"

class PassCommand : public ICommand
{
    public:
        PassCommand(const std::string& serverPassword);
        virtual ~PassCommand();

        virtual void	execute(Client& client,
                                const CommandMessage& message,
                                ClientManager& clients,
                                ChannelManager& channels);

    private:
        std::string		_serverPassword;

    private:
        PassCommand();
        PassCommand(const PassCommand& other);
        PassCommand&	operator=(const PassCommand& other);
};

#endif