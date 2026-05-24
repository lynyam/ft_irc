*This project has been created as part of the 42 curriculum by ynyamets, yurwang, lulmaruy.*

# ft_irc

## Description

`ft_irc` is a C++98 Internet Relay Chat server.

The goal is to implement a server compatible with a real IRC client. The server accepts multiple TCP clients, authenticates them with a connection password, allows users to set nicknames and usernames, join channels, send private messages, and manage channel operator commands.

The project focuses on:

- TCP/IP communication
- non-blocking sockets
- one `select()`-based event loop
- IRC command parsing
- channel and user state management
- clean C++98 architecture

## Instructions

### Build

```bash
make
````

### Run

```bash
./ircserv <port> <password>
```

Example:

```bash
./ircserv 6667 secret
```

### Clean

```bash
make clean
make fclean
make re
```

## Reference client

Reference client to be decided by the team.

Possible candidates:

* `irssi`
* HexChat
* `nc` for manual low-level testing

## Architecture

See:

```txt
docs/architecture.md
docs/team-contract.md
docs/protocol-notes.md
```

## Resources

* RFC 2812 — IRC Client Protocol
* RFC 1459 — Original IRC Protocol
* Beej's Guide to Network Programming
* Linux/macOS manual pages for `socket`, `bind`, `listen`, `accept`, `recv`, `send`, `select`, and `fcntl`
* [text](http://manpagesfr.free.fr/man/man2/select_tut.2.html)

## AI usage

AI was used as a learning and architecture-support tool.

It helped with:

* understanding TCP/IP concepts
* designing the project architecture
* defining team interfaces
* drafting documentation
* preparing implementation steps

All generated content must be reviewed, tested, and understood by the team before being used.

````