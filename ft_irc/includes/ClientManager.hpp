#ifndef CLIENT_MANAGER_HPP
# define CLIENT_MANAGER_HPP

# include <map>
# include <string>
# include "Client.hpp"

class ClientManager
{
    public:
        ClientManager();
        ~ClientManager();

        void							addClient(int fd);
        void							removeClient(int fd);

        Client*							getByFd(int fd);
        const Client*					getByFd(int fd) const;

        Client*							getByNickname(const std::string& nickname);
        const Client*					getByNickname(const std::string& nickname) const;

        bool							nicknameExists(const std::string& nickname) const;

        std::map<int, Client*>&			getAll();
        const std::map<int, Client*>&	getAll() const;

    private:
        std::map<int, Client*>			_clients;

    private:
        ClientManager(const ClientManager& other);
        ClientManager&					operator=(const ClientManager& other);
};

#endif