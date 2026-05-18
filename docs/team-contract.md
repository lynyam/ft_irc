## Luli — Channel and ChannelManager

### Ownership
Luli owns:
- `Channel`
- `ChannelManager`

### Required invariants
- A channel operator must also be a channel member.
- `removeClient(client)` must remove the client from members, operators, and invited users.
- `broadcast()` and `broadcastExcept()` must never call `send()`.
- They must only call `Client::appendOutput()`.

### Required methods
- `ChannelManager::getOrCreate(name)`
- `ChannelManager::removeClientFromAllChannels(client)`
- `Channel::broadcast(message)`
- `Channel::broadcastExcept(excluded, message)`