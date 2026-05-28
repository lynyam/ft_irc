# IRC Protocol Notes

## Purpose

This document contains shared notes about the IRC protocol behavior we need for `ft_irc`.

It is not a full RFC implementation guide.  
It only documents the subset required for our project.

---

# Message format

IRC is a text-based protocol.

Clients and servers exchange lines of text.

Each IRC line normally ends with:

```txt
\r\n
````

Example:

```txt
NICK leon\r\n
USER leon 0 * :Leon Nyametso\r\n
JOIN #general\r\n
PRIVMSG #general :hello\r\n
```

The parser should receive complete lines without the final `\r\n`.

Example:

```txt
NICK leon
```

not:

```txt
NICK leon\r\n
```

---

# TCP reminder

TCP is a byte stream.

One `recv()` does not equal one IRC command.

A client may send:

```txt
NICK leon\r\n
```

but the server may receive:

```txt
NICK le
```

then later:

```txt
on\r\n
```

Therefore, each `Client` must keep an input buffer.

The server must aggregate received bytes until a full line is available.

---

# Simplified IRC message structure

A simplified IRC line looks like:

```txt
COMMAND param1 param2 :trailing text
```

Examples:

```txt
NICK leon
```

```txt
USER leon 0 * :Leon Nyametso
```

```txt
PRIVMSG #general :hello everyone
```

---

# CommandMessage mapping

## NICK

Raw:

```txt
NICK leon
```

Parsed:

```txt
command  = NICK
params   = ["leon"]
trailing = ""
```

---

## USER

Raw:

```txt
USER leon 0 * :Leon Nyametso
```

Parsed:

```txt
command  = USER
params   = ["leon", "0", "*"]
trailing = "Leon Nyametso"
```

---

## JOIN

Raw:

```txt
JOIN #general
```

Parsed:

```txt
command  = JOIN
params   = ["#general"]
trailing = ""
```

---

## PRIVMSG

Raw:

```txt
PRIVMSG #general :hello everyone
```

Parsed:

```txt
command  = PRIVMSG
params   = ["#general"]
trailing = "hello everyone"
```

---

## MODE

Raw:

```txt
MODE #general +i
```

Parsed:

```txt
command  = MODE
params   = ["#general", "+i"]
trailing = ""
```

Raw:

```txt
MODE #general +o bob
```

Parsed:

```txt
command  = MODE
params   = ["#general", "+o", "bob"]
trailing = ""
```

---

# Registration commands

A client is considered registered when:

```txt
PASS accepted
NICK set
USER set
```

Required commands:

```txt
PASS <password>
NICK <nickname>
USER <username> 0 * :<realname>
```

Example:

```txt
PASS secret
NICK leon
USER leon 0 * :Leon Nyametso
```

After successful registration, the server should send welcome replies.

---

# PASS

Purpose:

```txt
Authenticate the client using the server password.
```

Example:

```txt
PASS secret
```

Behavior:

```txt
if password matches server password:
    client.passwordAccepted = true
else:
    return password error
```

The server password is passed at startup:

```bash
./ircserv <port> <password>
```

---

# NICK

Purpose:

```txt
Set the client nickname.
```

Example:

```txt
NICK leon
```

Checks:

```txt
nickname parameter exists
nickname is not already used
nickname format is valid
```

State change:

```txt
client.nickname = "leon"
```

---

# USER

Purpose:

```txt
Set username and realname.
```

Example:

```txt
USER leon 0 * :Leon Nyametso
```

State change:

```txt
client.username = "leon"
client.realname = "Leon Nyametso"
```

---

# JOIN

Purpose:

```txt
Join a channel.
```

Example:

```txt
JOIN #general
```

Behavior:

```txt
if client is not registered:
    return error

if channel does not exist:
    create channel
    add client
    make client operator

else:
    check channel rules
    add client
```

Channel rules may include:

```txt
invite-only
key/password
user limit
```

---

# PRIVMSG

Purpose:

```txt
Send a private message to a user or channel.
```

Examples:

```txt
PRIVMSG bob :hello
PRIVMSG #general :hello everyone
```

Behavior for user target:

```txt
find target client by nickname
append message to target output buffer
```

Behavior for channel target:

```txt
find target channel
check sender is allowed to send
broadcast message to channel members except sender
```

---

# PART

Purpose:

```txt
Leave a channel.
```

Example:

```txt
PART #general
```

Behavior:

```txt
remove client from channel
remove operator status if needed
remove channel if empty
```

---

# QUIT

Purpose:

```txt
Disconnect from server.
```

Example:

```txt
QUIT :bye
```

Behavior:

```txt
remove client from all channels
close connection
remove client from ClientManager
```

---

# PING / PONG

Real IRC clients may send:

```txt
PING :token
```

Server should reply:

```txt
PONG :token
```

This improves compatibility with real IRC clients.

---

# Channel operators

The first client to create a channel becomes operator.

Operators can use administrative commands:

```txt
KICK
INVITE
TOPIC
MODE
```

---

# KICK

Purpose:

```txt
Eject a client from a channel.
```

Example:

```txt
KICK #general bob :reason
```

Checks:

```txt
channel exists
sender is in channel
sender is operator
target user exists
target user is in channel
```

Behavior:

```txt
broadcast KICK message
remove target from channel
```

---

# INVITE

Purpose:

```txt
Invite a user to a channel.
```

Example:

```txt
INVITE bob #general
```

Checks:

```txt
channel exists
sender is operator
target user exists
```

Behavior:

```txt
mark target user as invited
send invite message to target
```

---

# TOPIC

Purpose:

```txt
View or change channel topic.
```

View topic:

```txt
TOPIC #general
```

Change topic:

```txt
TOPIC #general :new topic
```

Checks for changing topic:

```txt
channel exists
sender is in channel
if +t mode enabled:
    sender must be operator
```

---

# MODE

Purpose:

```txt
Change channel modes.
```

Required modes:

```txt
+i / -i
    set/remove invite-only channel

+t / -t
    set/remove topic restriction to operators

+k / -k
    set/remove channel key

+o / -o
    give/take channel operator privilege

+l / -l
    set/remove user limit
```

Examples:

```txt
MODE #general +i
MODE #general -i
MODE #general +t
MODE #general +k secret
MODE #general -k
MODE #general +o bob
MODE #general -o bob
MODE #general +l 10
MODE #general -l
```

Checks:

```txt
channel exists
sender is in channel
sender is operator
mode parameter is valid
required mode arguments exist
```

---

# Important project rules

Commands must never call:

```cpp
send()
recv()
close()
```

Commands must queue output:

```cpp
client.appendOutput(reply);
```

Only `Server` performs network I/O inside the `select()` event loop.

---

# Useful resources

* RFC 2812 — IRC Client Protocol
* RFC 1459 — Original IRC Protocol
* Beej's Guide to Network Programming

````