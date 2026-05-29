#include <iostream>
#include <string>
#include "Parser.hpp"
#include "CommandMessage.hpp"
#include "CommandDispatcher.hpp"
#include "ClientManager.hpp"
#include "ChannelManager.hpp"
#include "Client.hpp"

// ============================================================
// PARSER TESTS
// ============================================================

void printMessage(const CommandMessage& msg)
{
    std::cout << "  prefix:   [" << msg.getPrefix() << "]" << std::endl;
    std::cout << "  command:  [" << msg.getCommand() << "]" << std::endl;
    for (size_t i = 0; i < msg.paramCount(); i++)
        std::cout << "  param[" << i << "]: [" << msg.getParam(i) << "]" << std::endl;
    std::cout << "  trailing: [" << msg.getTrailing() << "]" << std::endl;
}

void testParser(const std::string& raw, const std::string& expectedCommand,
                size_t expectedParams, const std::string& expectedPrefix = "")
{
    CommandMessage msg = Parser::parse(raw);
    bool ok = (msg.getCommand() == expectedCommand)
           && (msg.paramCount() == expectedParams)
           && (msg.getPrefix() == expectedPrefix);
    std::cout << (ok ? "[PASS]" : "[FAIL]") << " parse(\"" << raw.substr(0, raw.size() - 2) << "\")" << std::endl;
    if (!ok)
        printMessage(msg);
}

void runParserTests()
{
    std::cout << "\n========== PARSER TESTS ==========" << std::endl;

    testParser("NICK alice\r\n",                              "NICK",    1);
    testParser("PASS secret\r\n",                             "PASS",    1);
    testParser("USER alice 0 * :Real Name\r\n",               "USER",    4);
    testParser("JOIN #general\r\n",                           "JOIN",    1);
    testParser("JOIN #general secretkey\r\n",                 "JOIN",    2);
    testParser("PART #general\r\n",                           "PART",    1);
    testParser("PART #general :Goodbye\r\n",                  "PART",    2);
    testParser("PRIVMSG #general :hello world\r\n",           "PRIVMSG", 2);
    testParser("PRIVMSG alice :hello\r\n",                    "PRIVMSG", 2);
    testParser("QUIT\r\n",                                    "QUIT",    0);
    testParser("QUIT :Bye\r\n",                               "QUIT",    1);
    testParser("KICK #general alice :reason\r\n",             "KICK",    3);
    testParser("INVITE alice #general\r\n",                   "INVITE",  2);
    testParser("TOPIC #general\r\n",                          "TOPIC",   1);
    testParser("TOPIC #general :new topic\r\n",               "TOPIC",   2);
    testParser("MODE #general +i\r\n",                        "MODE",    2);
    testParser("MODE #general +k secret\r\n",                 "MODE",    3);
    testParser("MODE #general +o alice\r\n",                  "MODE",    3);
    testParser("MODE #general +l 10\r\n",                     "MODE",    3);
    testParser(":alice!user@host PRIVMSG #general :hi\r\n",   "PRIVMSG", 2, "alice!user@host");
    testParser("\r\n",                                        "",        0);
    testParser("UNKNOWN\r\n",                                 "UNKNOWN", 0);
}

// ============================================================
// COMMAND TESTS
// ============================================================

void flushOutput(Client& client, bool print = true)
{
    if (client.hasPendingOutput())
    {
        if (print)
            std::cout << "  <<< " << client.getOutputBuffer();
        client.consumeOutput(client.getOutputBuffer().size());
    }
}

void testCommand(CommandDispatcher& dispatcher, Client& client,
                 const std::string& raw, const std::string& label)
{
    (void)label;
    std::cout << "  >>> " << raw;
    dispatcher.dispatch(client, raw);
    flushOutput(client);
}

void runCommandTests()
{
    std::cout << "\n========== COMMAND TESTS ==========" << std::endl;

    ClientManager clients;
    ChannelManager channels;
    CommandDispatcher dispatcher(clients, channels, "secret");

    clients.addClient(1);
    clients.addClient(2);
    Client* alice = clients.getByFd(1);
    Client* bob   = clients.getByFd(2);

    // --- PASS ---
    std::cout << "\n-- PASS --" << std::endl;
    testCommand(dispatcher, *alice, "PASS wrongpass\r\n",  "wrong password");
    testCommand(dispatcher, *alice, "PASS\r\n",            "no params");
    testCommand(dispatcher, *alice, "PASS secret\r\n",     "correct password");

    // --- NICK ---
    std::cout << "\n-- NICK --" << std::endl;
    testCommand(dispatcher, *alice, "NICK\r\n",            "no nickname");
    testCommand(dispatcher, *alice, "NICK alice\r\n",      "set nickname");

    // --- USER + registration ---
    std::cout << "\n-- USER --" << std::endl;
    testCommand(dispatcher, *alice, "USER alice 0 * :Alice Real\r\n", "register alice");

    // register bob too
    testCommand(dispatcher, *bob,   "PASS secret\r\n",     "bob pass");
    testCommand(dispatcher, *bob,   "NICK bob\r\n",        "bob nick");
    testCommand(dispatcher, *bob,   "USER bob 0 * :Bob\r\n", "register bob");

    // --- NICK duplicate ---
    std::cout << "\n-- NICK duplicate --" << std::endl;
    testCommand(dispatcher, *bob,   "NICK alice\r\n",      "duplicate nick");

    // --- JOIN ---
    std::cout << "\n-- JOIN --" << std::endl;
    testCommand(dispatcher, *alice, "JOIN\r\n",            "no params");
    testCommand(dispatcher, *alice, "JOIN #general\r\n",   "alice joins");
    testCommand(dispatcher, *bob,   "JOIN #general\r\n",   "bob joins");

    // --- TOPIC ---
    std::cout << "\n-- TOPIC --" << std::endl;
    testCommand(dispatcher, *alice, "TOPIC #general\r\n",          "view topic (empty)");
    testCommand(dispatcher, *alice, "TOPIC #general :hello\r\n",   "set topic");
    testCommand(dispatcher, *alice, "TOPIC #general\r\n",          "view topic");

    // --- PRIVMSG to channel ---
    std::cout << "\n-- PRIVMSG --" << std::endl;
    testCommand(dispatcher, *alice, "PRIVMSG #general :hello everyone\r\n", "msg to channel");
    testCommand(dispatcher, *alice, "PRIVMSG bob :hello bob\r\n",           "msg to user");
    testCommand(dispatcher, *alice, "PRIVMSG unknown :hello\r\n",           "msg to unknown");

    // --- INVITE ---
    std::cout << "\n-- INVITE --" << std::endl;
    testCommand(dispatcher, *alice, "MODE #general +i\r\n",        "set invite-only");
    testCommand(dispatcher, *alice, "INVITE bob #general\r\n",     "invite bob");

    // --- KICK ---
    std::cout << "\n-- KICK --" << std::endl;
    testCommand(dispatcher, *alice, "KICK #general bob :bye\r\n",  "kick bob");

    // --- MODE ---
    std::cout << "\n-- MODE --" << std::endl;
    testCommand(dispatcher, *alice, "MODE #general +k secret\r\n", "set key");
    testCommand(dispatcher, *alice, "MODE #general -k\r\n",        "remove key");
    testCommand(dispatcher, *alice, "MODE #general +l 10\r\n",     "set limit");
    testCommand(dispatcher, *alice, "MODE #general -l\r\n",        "remove limit");
    testCommand(dispatcher, *alice, "MODE #general +t\r\n",        "set topic protect");

    // --- PART ---
    std::cout << "\n-- PART --" << std::endl;
    testCommand(dispatcher, *alice, "PART #general :bye\r\n",      "alice parts");

    // --- QUIT ---
    std::cout << "\n-- QUIT --" << std::endl;
    testCommand(dispatcher, *alice, "QUIT :goodbye\r\n",           "alice quits");

    // --- unknown command ---
    std::cout << "\n-- UNKNOWN --" << std::endl;
    testCommand(dispatcher, *bob,   "UNKNOWN\r\n",                 "unknown command");
}

// ============================================================
// MAIN
// ============================================================

int main()
{
    runParserTests();
    runCommandTests();
    std::cout << "\nDone!" << std::endl;
    return 0;
}