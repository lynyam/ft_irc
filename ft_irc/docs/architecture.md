# ft_irc Architecture

## Project goal

`ft_irc` is a C++98 IRC server.

The executable is:

```bash
./ircserv <port> <password>
````

The server must handle multiple IRC clients over TCP/IP, use non-blocking file descriptors, and centralize network I/O through one `select()` event loop.

## Core rule

Only `Server` is allowed to call network I/O functions:

* `socket`
* `setsockopt`
* `bind`
* `listen`
* `accept`
* `recv`
* `send`
* `close`
* `select`
* `fcntl`

Command classes must never call `send()` or `recv()` directly.

Commands only queue replies using:

```cpp
client.appendOutput(reply);
```

The `Server` later sends queued data when `select()` marks the fd writable.

## Global flow

```txt
TCP bytes received
    ↓
Server::readFromClient()
    ↓
Client::_inputBuffer
    ↓
Client::popLine()
    ↓
CommandDispatcher
    ↓
Parser
    ↓
CommandMessage
    ↓
specific command class
    ↓
ClientManager / ChannelManager / Channel
    ↓
Client::_outputBuffer
    ↓
Server::writeToClient()
```

## Class responsibilities

### Server

Owns the network lifecycle:

* create listening socket
* bind/listen
* set non-blocking mode
* run the `select()` loop
* accept clients
* read client bytes
* write pending output
* disconnect clients safely

### Client

Represents one TCP connection and IRC user state:

* socket fd
* input buffer
* output buffer
* password accepted state
* nickname
* username
* realname
* registered state

### ClientManager

Owns all connected clients.

Responsibilities:

* add client by fd
* remove client by fd
* find client by fd
* find client by nickname
* expose clients for the `select()` loop

### Channel

Represents one IRC channel.

Responsibilities:

* members
* operators
* invited users
* topic
* modes
* broadcast helpers

Important invariant:

```txt
operators ⊆ channel members
```

### ChannelManager

Owns all existing channels.

Responsibilities:

* find channel
* create channel
* get or create channel
* remove empty channel
* remove a disconnected client from all channels

### Parser

Transforms a complete IRC line into a `CommandMessage`.

It must not check business rules.

### CommandMessage

Stores parsed IRC data:

* prefix
* command
* params
* trailing text

### CommandDispatcher

Routes a parsed command to the correct command object.

Example:

```txt
JOIN    -> JoinCommand
PRIVMSG -> PrivmsgCommand
NICK    -> NickCommand
```

### ICommand

Common interface for every IRC command.

### Command classes

Implement IRC behavior.

Examples:

* `PassCommand`
* `NickCommand`
* `UserCommand`
* `JoinCommand`
* `PrivmsgCommand`
* `KickCommand`
* `InviteCommand`
* `TopicCommand`
* `ModeCommand`
* `PingCommand`

### ReplyBuilder

Builds IRC reply strings in one centralized place.

## Buffer rules

### Input buffer

Used for data received from a client.

TCP is a byte stream, so one `recv()` can contain:

* half a command
* one command
* many commands
* one command plus part of the next one

Therefore, `Client::_inputBuffer` stores bytes until a complete line is available.

### Output buffer

Used for data the server wants to send to a client.

`send()` may send only part of the buffer, so the unsent part must remain in `Client::_outputBuffer`.

## Disconnect rule

Before deleting a `Client`, the server must remove it from all channels:

```cpp
_channels.removeClientFromAllChannels(client);
_clients.removeClient(fd);
close(fd);
```

This prevents dangling `Client*` inside channels (pointeur that point to non existant memory).

````