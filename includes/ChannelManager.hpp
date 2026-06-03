#ifndef CHANNEL_MANAGER_HPP
# define CHANNEL_MANAGER_HPP

# include <map>
# include <string>
# include "Channel.hpp"

class Client;

class ChannelManager
{
    public:
        ChannelManager();
        ~ChannelManager();

        Channel*								get(const std::string& name);
        const Channel*							get(const std::string& name) const;

        Channel*								create(const std::string& name);
        Channel*								getOrCreate(const std::string& name);

        bool									exists(const std::string& name) const;

        void									remove(const std::string& name);
        void									removeIfEmpty(const std::string& name);

        void                                    removeClientFromAllChannels(Client* client);
        void                                    broadcastToClientChannels(Client* client, const std::string& message);

        std::map<std::string, Channel*>&			getAll();
        const std::map<std::string, Channel*>&	getAll() const;

    private:
        std::map<std::string, Channel*>			_channels;

    private:
        ChannelManager(const ChannelManager& other);
        ChannelManager&							operator=(const ChannelManager& other);
};

#endif