#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <cstddef>

class Client
{
    public:
        Client(int fd);
        ~Client();

        int					getFd() const;

        void				appendInput(const std::string& data);
        bool				hasCompleteLine() const;
        std::string			popLine();

        void				appendOutput(const std::string& data);
        bool				hasPendingOutput() const;
        const std::string&	getOutputBuffer() const;
        void				consumeOutput(size_t size);

        void				setPasswordAccepted(bool value);
        bool				isPasswordAccepted() const;

        void				setNickname(const std::string& nickname);
        void				setUsername(const std::string& username);
        void				setRealname(const std::string& realname);

        const std::string&	getNickname() const;
        const std::string&	getUsername() const;
        const std::string&	getRealname() const;

        bool				hasNickname() const;
        bool				hasUsername() const;
        bool				canRegister() const;
        bool				isRegistered() const;
        void				markRegistered();

        std::string			getPrefix() const;

    private:
        int					_fd;
        std::string			_inputBuffer;
        std::string			_outputBuffer;

        bool				_passwordAccepted;
        bool				_registered;

        std::string			_nickname;
        std::string			_username;
        std::string			_realname;

    private:
        Client();
        Client(const Client& other);
        Client&				operator=(const Client& other);
};
#endif