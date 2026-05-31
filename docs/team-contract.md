# ft_irc Team Contract

## Ownership

### Leon

Owns:

- `Server`
- `Client`
- `ClientManager`
- network loop
- non-blocking sockets
- `select()`
- client connection lifecycle
- input/output buffers

### Luli

Owns:

- `Channel`
- `ChannelManager`
- channel members
- channel operators
- invited users
- topics
- channel modes

### Yurong

Owns:

- `Parser`
- `CommandMessage`
- `CommandDispatcher`
- command classes
- `ReplyBuilder`
- IRC numeric replies

## Shared architecture rules

### Rule 1 — Network I/O ownership

Only `Server` may call:

- `accept`
- `recv`
- `send`
- `close`
- `select`
- `fcntl`

Commands and channels must never call `send()` directly.

### Rule 2 — Output rule

To send a message to a client, use:

```cpp
client.appendOutput(message);
````

The server event loop will send it later.

### Rule 3 — Parser responsibility

The parser only transforms raw IRC lines into `CommandMessage`.

It must not:

* check whether a channel exists
* check whether a nickname is already used
* check operator permissions
* modify clients
* modify channels

### Rule 4 — Command responsibility

Commands implement IRC behavior.

Commands may:

* read and modify the current `Client`
* use `ClientManager`
* use `ChannelManager`
* call `Client::appendOutput()`

Commands must not:

* call `send()`
* call `recv()`
* call `close()`
* access raw sockets directly

### Rule 5 — Channel responsibility

`Channel` manages one room.

`ChannelManager` manages all rooms.

A channel operator must also be a channel member.

When a client leaves or disconnects, it must be removed from:

* members
* operators
* invited users
Required invariants
- A channel operator must also be a channel member.
- `removeClient(client)` must remove the client from members, operators, and invited users.
- `broadcast()` and `broadcastExcept()` must never call `send()`.
- They must only call `Client::appendOutput()`.
Required methods
- `ChannelManager::getOrCreate(name)`
- `ChannelManager::removeClientFromAllChannels(client)`
- `Channel::broadcast(message)`
- `Channel::broadcastExcept(excluded, message)`

### Rule 6 — Disconnect cleanup

Before deleting a client, `Server` must call:

```cpp
_channels.removeClientFromAllChannels(client);
```

### Rule 7 — Interface changes

Public method signatures in `.hpp` files should not be changed without team agreement.

Private helper methods can be added freely by the file owner.