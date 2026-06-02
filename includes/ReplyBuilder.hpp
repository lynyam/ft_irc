#ifndef REPLY_BUILDER_HPP
# define REPLY_BUILDER_HPP

# include <string>

class Client;

class ReplyBuilder
{
    public:
        static std::string	welcome(const std::string& nick);
        static std::string	yourHost(const std::string& nick);
        static std::string	created(const std::string& nick);

        static std::string	errNeedMoreParams(const std::string& nick,
                                            const std::string& command);
        static std::string	errAlreadyRegistered(const std::string& nick);
        static std::string	errPasswordMismatch();
        static std::string	errNoNicknameGiven();
        static std::string	errNicknameInUse(const std::string& nick);
        static std::string	errNotRegistered(const std::string& nick);
        static std::string	errNoSuchNick(const std::string& nick,
                                        const std::string& target);
        static std::string	errNoSuchChannel(const std::string& nick,
                                            const std::string& channel);
        static std::string	errCannotSendToChan(const std::string& nick,
                                            const std::string& channel);
        static std::string	errChanOPrivsNeeded(const std::string& nick,
                                            const std::string& channel);
        static std::string	errUserOnChannel(const std::string& nick,
                                            const std::string& target,
                                            const std::string& channel);
        static std::string	errInviteOnlyChan(const std::string& nick,
                                            const std::string& channel);
        static std::string	errBadChannelKey(const std::string& nick,
                                            const std::string& channel);
        static std::string	errChannelIsFull(const std::string& nick,
                                            const std::string& channel);
        static std::string  errNotOnChannel(const std::string& nick,
                                    const std::string& channel);
        static std::string  errUserNotInChannel(const std::string& nick,
                                        const std::string& target,
                                        const std::string& channel);
        static std::string  errUnknownCommand(const std::string& nick,
                                        const std::string& command);
        static std::string  errErroneousNickname(const std::string& nick);
        static std::string  errUnknownMode(const std::string& nick, char mode);
        static std::string  errNoTextToSend(const std::string& nick);

        static std::string	join(const Client& client,
                                const std::string& channel);
        static std::string	part(const Client& client,
                                const std::string& channel,
                                const std::string& reason);
        static std::string	privmsg(const Client& from,
                                    const std::string& target,
                                    const std::string& text);
        static std::string	noTopic(const std::string& nick,
                                const std::string& channel);
        static std::string	topic(const std::string& nick,
                                const std::string& channel,
                                const std::string& topic);
        static std::string	namReply(const std::string& nick,
                                    const std::string& channel,
                                    const std::string& names);
        static std::string	endOfNames(const std::string& nick,
                                    const std::string& channel);
        static std::string	mode(const Client& client,
                                const std::string& channel,
                                const std::string& modes,
                                const std::string& args);
        static std::string  mode(const std::string& serverName, const std::string& channel,
                                const std::string& modes, const std::string& args);
        static std::string	pong(const std::string& token);
        static void         tryRegister(Client& client);
        static std::string	quit(const Client& client, const std::string& reason);
        static std::string	kick(const Client& client, const std::string& channel,
                                const std::string& target, const std::string& reason);
        static std::string	invite(const Client& client, const std::string& target,
                                const std::string& channel);
        static std::string  rplInviting(const std::string& nick, const std::string& target,
                                const std::string& channel);
        static std::string	setTopic(const Client& client, const std::string& channel,
                                const std::string& topic);
        static std::string	nick(const std::string& oldPrefix, const std::string& newNick);
        static std::string	errorClosingLink(const std::string& nick, const std::string& reason);

    private:
        ReplyBuilder();
        ReplyBuilder(const ReplyBuilder& other);
        ReplyBuilder&	operator=(const ReplyBuilder& other);

        static std::string  resolveNick(const std::string& nick);
};

#endif