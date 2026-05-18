#include "Parser.hpp"
#include <sstream>
/*
    ToDO Yurong
    - It is a minimum parser free to replace/improve
     with your parser
        first word       -> command
        normal words     -> params
        words after ':'  -> trailing       
        example: PRIVMSG #general :hello world
        command  = PRIVMSG
        params   = #general
        trailing = hello world
*/
CommandMessage	Parser::parse(const std::string& line)
{
	CommandMessage			message;
	std::istringstream		stream(line);
	std::string				token;

	if (!(stream >> token))
		return (message);
	message.setCommand(token);

	while (stream >> token)
	{
		if (!token.empty() && token[0] == ':')
		{
			std::string	trailing;
			std::string	rest;

			trailing = token.substr(1);
			while (stream >> rest)
				trailing += " " + rest;
			message.setTrailing(trailing);
			break ;
		}
		message.addParam(token);
	}
	return (message);
}