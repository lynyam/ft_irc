#include "Parser.hpp"

CommandMessage Parser::parse(const std::string& line)
{
    CommandMessage msg;
    if (line.empty())
        return msg;

    const std::string& tmp = line;
    size_t pos = 0;

    // get prefix
    if (!tmp.empty() && tmp[0] == ':')
    {
        size_t prefix_end = tmp.find(' ');
        if (prefix_end == std::string::npos)
            return msg;
        msg.setPrefix(tmp.substr(1, prefix_end - 1));
        pos = prefix_end + 1;
    }

    while (pos < tmp.size() && tmp[pos] == ' ')
        ++pos;
    if (pos >= tmp.size())
        return msg;

    // get command
    size_t command_end = tmp.find(' ', pos);
    if (command_end == std::string::npos)
    {
        msg.setCommand(tmp.substr(pos));
        return msg;
    }
    msg.setCommand(tmp.substr(pos, command_end - pos));
    pos = command_end + 1;

    // get params
    while (pos < tmp.size())
    {
        while (pos < tmp.size() && tmp[pos] == ' ')
            ++pos;
        if (pos >= tmp.size())
            break;
        if (tmp[pos] == ':')
        {
            msg.setTrailing(tmp.substr(pos + 1));
            break;
        }
        size_t next_space = tmp.find(' ', pos);
        if (next_space == std::string::npos)
        {
            msg.addParam(tmp.substr(pos));
            break;
        }
        msg.addParam(tmp.substr(pos, next_space - pos));
        pos = next_space + 1;
    }
    return msg;
}