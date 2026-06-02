#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <set>
# include <cstddef>

class Client;

class Channel
{
    public:
        Channel(const std::string& name);
        ~Channel();

        const std::string&	getName() const;

        void				addClient(Client* client);
        void				removeClient(Client* client);
        bool				hasClient(Client* client) const;
        bool				isEmpty() const;
        size_t				getClientCount() const;

        void				addOperator(Client* client);
        void				removeOperator(Client* client);
        bool				isOperator(Client* client) const;

        void				invite(Client* client);
        void				removeInvite(Client* client);
        bool				isInvited(Client* client) const;

        void				setTopic(const std::string& topic);
        const std::string&	getTopic() const;
        bool				hasTopic() const;

        void				setInviteOnly(bool value);
        bool				isInviteOnly() const;

        void				setTopicProtected(bool value);
        bool				isTopicProtected() const;

        void				setKey(const std::string& key);
        void				removeKey();
        bool				hasKey() const;
        bool				checkKey(const std::string& key) const;

        void				setUserLimit(size_t limit);
        void				removeUserLimit();
        bool				hasUserLimit() const;
        bool				isFull() const;
        size_t				getUserLimit() const;

        void				broadcast(const std::string& message);
        void				broadcastExcept(Client* excluded,
                                const std::string& message);

        std::string			buildNamesList() const;
        std::string			buildModeString() const;

        Client*             getFirstMember() const;
        Client*             getFirstMemberExcept(Client* excluded) const;
        bool                hasOperator() const;
        size_t              getOperatorCount() const;
        const std::set<Client*>& getClients() const;
    private:
        std::string			_name;
        std::string			_topic;
        bool				_hasTopic;

        std::set<Client*>	_clients;
        std::set<Client*>	_operators;
        std::set<Client*>	_invited;

        bool				_inviteOnly;
        bool				_topicProtected;

        bool				_hasKey;
        std::string			_key;

        bool				_hasUserLimit;
        size_t				_userLimit;

    private:
        Channel();
        Channel(const Channel& other);
        Channel&			operator=(const Channel& other);
    };

#endif
