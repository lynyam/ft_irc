#ifndef PARSER_HPP
# define PARSER_HPP

# include <string>
# include "CommandMessage.hpp"

class Parser
{
    public:
        static CommandMessage	parse(const std::string& line);

    private:
        Parser();
        Parser(const Parser& other);
        Parser&	operator=(const Parser& other);
};

#endif