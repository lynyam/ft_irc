#include "ReplyBuilder.hpp"
#include "Client.hpp"

std::string ReplyBuilder::resolveNick(const std::string& nick)
{
    return nick.empty() ? "*" : nick;
}

std::string ReplyBuilder::welcome(const std::string& nick)
{
    const std::string n = resolveNick(nick);
    return ":server 001 " + n + " :Welcome to the IRC server " + n + "\r\n";
}

std::string ReplyBuilder::yourHost(const std::string& nick)
{
    return ":server 002 " + resolveNick(nick) + " :Your host is ircserv\r\n";
}

std::string ReplyBuilder::created(const std::string& nick)
{
    return ":server 003 " + resolveNick(nick) + " :This server was created today\r\n";
}

std::string ReplyBuilder::errNeedMoreParams(const std::string& nick, const std::string& command)
{
    return ":server 461 " + resolveNick(nick) + " " + command + " :Not enough parameters\r\n";
}

std::string ReplyBuilder::errAlreadyRegistered(const std::string& nick)
{
    return ":server 462 " + resolveNick(nick) + " :You may not reregister\r\n";
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
    return ":server 451 " + resolveNick(nick) + " :You have not registered\r\n";
}

std::string ReplyBuilder::errNoSuchNick(const std::string& nick, const std::string& target)
{
    return ":server 401 " + resolveNick(nick) + " " + target + " :No such nick\r\n";
}

std::string ReplyBuilder::errNoSuchChannel(const std::string& nick, const std::string& channel)
{
    return ":server 403 " + resolveNick(nick) + " " + channel + " :No such channel\r\n";
}

std::string ReplyBuilder::errCannotSendToChan(const std::string& nick, const std::string& channel)
{
    return ":server 404 " + resolveNick(nick) + " " + channel + " :Cannot send to channel\r\n";
}

std::string ReplyBuilder::errChanOPrivsNeeded(const std::string& nick, const std::string& channel)
{
    return ":server 482 " + resolveNick(nick) + " " + channel + " :You're not channel operator\r\n";
}

std::string ReplyBuilder::errUserOnChannel(const std::string& nick, const std::string& target, const std::string& channel)
{
    return ":server 443 " + resolveNick(nick) + " " + target + " " + channel + " :is already on channel\r\n";
}

std::string ReplyBuilder::errInviteOnlyChan(const std::string& nick, const std::string& channel)
{
    return ":server 473 " + resolveNick(nick) + " " + channel + " :Cannot join channel (+i)\r\n";
}

std::string ReplyBuilder::errBadChannelKey(const std::string& nick, const std::string& channel)
{
    return ":server 475 " + resolveNick(nick) + " " + channel + " :Cannot join channel (+k)\r\n";
}

std::string ReplyBuilder::errChannelIsFull(const std::string& nick, const std::string& channel)
{
    return ":server 471 " + resolveNick(nick) + " " + channel + " :Cannot join channel (+l)\r\n";
}

std::string ReplyBuilder::errNotOnChannel(const std::string& nick, const std::string& channel)
{
    return ":server 442 " + resolveNick(nick) + " " + channel + " :You're not on that channel\r\n";
}

std::string ReplyBuilder::errUserNotInChannel(const std::string& nick, const std::string& target, const std::string& channel)
{
    return ":server 441 " + resolveNick(nick) + " " + target + " " + channel + " :They aren't on that channel\r\n";
}

std::string ReplyBuilder::join(const Client& client, const std::string& channel)
{
    return client.getPrefix() + " JOIN " + channel + "\r\n";
}

std::string ReplyBuilder::part(const Client& client, const std::string& channel, const std::string& reason)
{
    return client.getPrefix() + " PART " + channel + " :" + reason + "\r\n";
}

std::string ReplyBuilder::privmsg(const Client& from, const std::string& target, const std::string& text)
{
    return from.getPrefix() + " PRIVMSG " + target + " :" + text + "\r\n";
}

std::string ReplyBuilder::noTopic(const std::string& nick, const std::string& channel)
{
    return ":server 331 " + resolveNick(nick) + " " + channel + " :No topic is set\r\n";
}

std::string ReplyBuilder::topic(const std::string& nick, const std::string& channel, const std::string& topic)
{
    return ":server 332 " + resolveNick(nick) + " " + channel + " :" + topic + "\r\n";
}

std::string ReplyBuilder::namReply(const std::string& nick, const std::string& channel, const std::string& names)
{
    return ":server 353 " + resolveNick(nick) + " = " + channel + " :" + names + "\r\n";
}

std::string ReplyBuilder::endOfNames(const std::string& nick, const std::string& channel)
{
    return ":server 366 " + resolveNick(nick) + " " + channel + " :End of /NAMES list\r\n";
}

std::string ReplyBuilder::mode(const Client& client, const std::string& channel, const std::string& modes, const std::string& args)
{
    return client.getPrefix() + " MODE " + channel + " " + modes + (args.empty() ? "" : " " + args) + "\r\n";
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

std::string ReplyBuilder::quit(const Client& client, const std::string& reason)
{
    return client.getPrefix() + " QUIT :" + reason + "\r\n";
}

std::string ReplyBuilder::kick(const Client& client, const std::string& channel,
                               const std::string& target, const std::string& reason)
{
    return client.getPrefix() + " KICK " + channel + " " + target + " :" + reason + "\r\n";
}

std::string ReplyBuilder::invite(const Client& client, const std::string& target,
                                 const std::string& channel)
{
    return client.getPrefix() + " INVITE " + target + " :" + channel + "\r\n";
}

std::string ReplyBuilder::rplInviting(const std::string& nick, const std::string& target,
                                      const std::string& channel)
{
    return ":server 341 " + resolveNick(nick) + " " + target + " " + channel + "\r\n";
}

std::string ReplyBuilder::setTopic(const Client& client, const std::string& channel,
                                   const std::string& topic)
{
    return client.getPrefix() + " TOPIC " + channel + " :" + topic + "\r\n";
}

std::string ReplyBuilder::nick(const std::string& oldPrefix, const std::string& newNick)
{
    return oldPrefix + " NICK :" + newNick + "\r\n";
}

std::string ReplyBuilder::errorClosingLink(const std::string& nick, const std::string& reason)
{
    return "ERROR :Closing Link: " + resolveNick(nick) + " (" + reason + ")\r\n";
}

std::string ReplyBuilder::errUnknownCommand(const std::string& nick, const std::string& command)
{
    return ":server 421 " + resolveNick(nick) + " " + command + " :Unknown command\r\n";
}

std::string ReplyBuilder::errErroneousNickname(const std::string& nick)
{
    return ":server 432 * " + nick + " :Erroneous nickname\r\n";
}

std::string ReplyBuilder::errUnknownMode(const std::string& nick, char mode)
{
    return ":server 472 " + resolveNick(nick) + " " + mode + " :is unknown mode char to me\r\n";
}

std::string ReplyBuilder::errNoTextToSend(const std::string& nick)
{
    return ":server 412 " + resolveNick(nick) + " :No text to send\r\n";
}
