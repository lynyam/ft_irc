#include "ReplyBuilder.hpp"
#include "Client.hpp"

std::string ReplyBuilder::welcome(const std::string& nick)
{
    return ":server 001 " + nick + " :Welcome to the IRC server " + nick + "\r\n";
}

std::string ReplyBuilder::yourHost(const std::string& nick)
{
    return ":server 002 " + nick + " :Your host is ircserv\r\n";
}

std::string ReplyBuilder::created(const std::string& nick)
{
    return ":server 003 " + nick + " :This server was created today\r\n";
}

std::string ReplyBuilder::errNeedMoreParams(const std::string& nick, const std::string& command)
{
    return ":server 461 " + nick + " " + command + " :Not enough parameters\r\n";
}

std::string ReplyBuilder::errAlreadyRegistered(const std::string& nick)
{
    return ":server 462 " + nick + " :You may not reregister\r\n";
}

std::string ReplyBuilder::errPasswordMismatch()
{
    return ":server 464 * :Password incorrect\r\n";
}

std::string ReplyBuilder::errNoNicknameGiven()
{
    return ":server 431 * :No nickname given\r\n";
}

std::string ReplyBuilder::errNicknameInUse(const std::string& nick)
{
    return ":server 433 * " + nick + " :Nickname is already in use\r\n";
}

std::string ReplyBuilder::errNotRegistered(const std::string& nick)
{
    return ":server 451 " + nick + " :You have not registered\r\n";
}

std::string ReplyBuilder::errNoSuchNick(const std::string& nick, const std::string& target)
{
    return ":server 401 " + nick + " " + target + " :No such nick\r\n";
}

std::string ReplyBuilder::errNoSuchChannel(const std::string& nick, const std::string& channel)
{
    return ":server 403 " + nick + " " + channel + " :No such channel\r\n";
}

std::string ReplyBuilder::errCannotSendToChan(const std::string& nick, const std::string& channel)
{
    return ":server 404 " + nick + " " + channel + " :Cannot send to channel\r\n";
}

std::string ReplyBuilder::errChanOPrivsNeeded(const std::string& nick, const std::string& channel)
{
    return ":server 482 " + nick + " " + channel + " :You're not channel operator\r\n";
}

std::string ReplyBuilder::errUserOnChannel(const std::string& nick, const std::string& target, const std::string& channel)
{
    return ":server 443 " + nick + " " + target + " " + channel + " :is already on channel\r\n";
}

std::string ReplyBuilder::errInviteOnlyChan(const std::string& nick, const std::string& channel)
{
    return ":server 473 " + nick + " " + channel + " :Cannot join channel (+i)\r\n";
}

std::string ReplyBuilder::errBadChannelKey(const std::string& nick, const std::string& channel)
{
    return ":server 475 " + nick + " " + channel + " :Cannot join channel (+k)\r\n";
}

std::string ReplyBuilder::errChannelIsFull(const std::string& nick, const std::string& channel)
{
    return ":server 471 " + nick + " " + channel + " :Cannot join channel (+l)\r\n";
}

std::string ReplyBuilder::errNotOnChannel(const std::string& nick, const std::string& channel)
{
    return ":server 442 " + nick + " " + channel + " :You're not on that channel\r\n";
}

std::string ReplyBuilder::errUserNotInChannel(const std::string& nick, const std::string& target, const std::string& channel)
{
    return ":server 441 " + nick + " " + target + " " + channel + " :They aren't on that channel\r\n";
}

std::string ReplyBuilder::join(const Client& client, const std::string& channel)
{
    return ":" + client.getPrefix() + " JOIN " + channel + "\r\n";
}

std::string ReplyBuilder::part(const Client& client, const std::string& channel, const std::string& reason)
{
    return ":" + client.getPrefix() + " PART " + channel + " :" + reason + "\r\n";
}

std::string ReplyBuilder::privmsg(const Client& from, const std::string& target, const std::string& text)
{
    return ":" + from.getPrefix() + " PRIVMSG " + target + " :" + text + "\r\n";
}

std::string ReplyBuilder::topic(const std::string& nick, const std::string& channel, const std::string& topic)
{
    return ":server TOPIC " + nick + " " + channel + " :" + topic + "\r\n";
}

std::string ReplyBuilder::namReply(const std::string& nick, const std::string& channel, const std::string& names)
{
    return ":server 353 " + nick + " = " + channel + " :" + names + "\r\n";
}

std::string ReplyBuilder::endOfNames(const std::string& nick, const std::string& channel)
{
    return ":server 366 " + nick + " " + channel + " :End of /NAMES list\r\n";
}

std::string ReplyBuilder::mode(const Client& client, const std::string& channel, const std::string& modes, const std::string& args)
{
    return ":" + client.getPrefix() + " MODE " + channel + " " + modes + (args.empty() ? "" : " " + args) + "\r\n";
}

std::string ReplyBuilder::mode(const std::string& serverName, const std::string& channel, const std::string& modes, const std::string& args)
{
    return ":" + serverName + " MODE " + channel + " " + modes
           + (args.empty() ? "" : " " + args) + "\r\n";
}

std::string ReplyBuilder::pong(const std::string& token)
{
    return ":server PONG server :" + token + "\r\n";
}