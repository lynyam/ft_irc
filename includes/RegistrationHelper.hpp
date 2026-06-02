#ifndef REGISTRATION_HELPER_HPP
# define REGISTRATION_HELPER_HPP

# include "Client.hpp"
# include "ReplyBuilder.hpp"

inline void tryRegister(Client& client)
{
    if (!client.isRegistered() && client.canRegister())
    {
        client.markRegistered();
        client.appendOutput(ReplyBuilder::welcome(client.getNickname()));
        client.appendOutput(ReplyBuilder::yourHost(client.getNickname()));
        client.appendOutput(ReplyBuilder::created(client.getNickname()));
    }
}

#endif
