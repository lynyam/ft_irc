#ifndef COMMAND_MESSAGE_HPP
# define COMMAND_MESSAGE_HPP

# include <string>
# include <vector>
# include <cstddef>

class CommandMessage
{
    public:
        CommandMessage();
        ~CommandMessage();

        const std::string&				getPrefix() const;
        const std::string&				getCommand() const;
        const std::vector<std::string>&	getParams() const;
        const std::string&				getTrailing() const;

        void							setPrefix(const std::string& prefix);
        void							setCommand(const std::string& command);
        void							addParam(const std::string& param);
        void							setTrailing(const std::string& trailing);

        bool							hasParam(size_t index) const;
        const std::string&				getParam(size_t index) const;
        size_t							paramCount() const;
        bool							hasTrailing() const;

    private:
        std::string						_prefix;
        std::string						_command;
        std::vector<std::string>		_params;
        std::string						_trailing;
        bool							_hasTrailing;
};

#endif