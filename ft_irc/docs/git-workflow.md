
````md
# Git Workflow

## Branch strategy

The project uses the following branch model:

```txt
main
  ↑
  stable releases only
  ↑
dev
  ↑
  integration branch
  ↑
feature/*
  individual work branches
````

---

## Branch roles

### `main`

`main` contains only stable versions of the project.

Rules:

* no direct commits;
* only merge from `dev`;
* should always compile;
* should represent a version close to evaluation-ready.

---

### `dev`

`dev` is the integration branch.

Rules:

* no direct commits unless agreed by the team;
* receives Pull Requests from `feature/*` branches;
* must compile after every merge;
* used for integration testing.

---

### `feature/*`

Each teammate works on a dedicated feature branch.

Current ownership:

```txt
feature/server-client-manager
    Owner: Léon
    Scope:
    - Server
    - Client
    - ClientManager
    - network loop
    - non-blocking sockets
    - select()
    - connection lifecycle

feature/channel-manager
    Owner: Luli
    Scope:
    - Channel
    - ChannelManager
    - members
    - operators
    - invites
    - topics
    - channel modes

feature/parser-command-system
    Owner: Yurong
    Scope:
    - Parser
    - CommandMessage
    - CommandDispatcher
    - command classes
    - ReplyBuilder
    - IRC replies

feature/docs
    Scope:
    - README
    - architecture documentation
    - team contract
    - protocol notes
    - Git workflow

feature/replies
    Scope:
    - optional branch for IRC numeric replies if separated from command system
```

---

## Daily workflow

Before starting work:

```bash
git checkout feature/<your-branch>
git pull origin feature/<your-branch>
```

After coding:

```bash
make fclean
make
```

If compilation succeeds:

```bash
git status
git add .
git commit -m "type: short description"
git push
```

Example:

```bash
git commit -m "feat: implement client input buffer"
```

---

## Commit message convention

Use simple prefixes:

```txt
chore: project setup, tooling, configuration
docs: documentation changes
feat: new feature
fix: bug fix
refactor: code cleanup without behavior change
test: test files or test scenarios
build: Makefile or build system changes
```

Examples:

```bash
git commit -m "feat: implement non-blocking server socket"
git commit -m "fix: remove client from channels on disconnect"
git commit -m "docs: add IRC protocol notes"
git commit -m "refactor: simplify channel mode helpers"
```

---

## Pull Request process

All feature branches must be merged into `dev` through a Pull Request.

Example:

```txt
feature/channel-manager -> dev
feature/parser-command-system -> dev
feature/server-client-manager -> dev
```

No direct merge into `main`.

---

## Pull Request checklist

Before opening a Pull Request:

* [ ] The branch compiles with `make fclean && make`.
* [ ] The code uses C++98.
* [ ] No generated files are committed.
* [ ] No object files are committed.
* [ ] No binary file such as `ircserv` is committed.
* [ ] Public `.hpp` interfaces were not changed without team agreement.
* [ ] Commands do not call `send()`, `recv()`, or `close()`.
* [ ] Parser does not modify clients or channels.
* [ ] Channel logic uses `Client::appendOutput()` instead of `send()`.
* [ ] The owner can explain the code during evaluation.

---

## Review checklist

When reviewing a Pull Request, check:

### General

* Does the project compile?
* Is the code readable?
* Are responsibilities respected?
* Are there avoidable crashes?
* Are memory allocations released correctly?

### Server / Client / ClientManager

* Is all network I/O inside `Server`?
* Are sockets non-blocking?
* Is `select()` used before `accept()`, `recv()`, and `send()`?
* Are partial receives handled through `Client::_inputBuffer`?
* Are partial sends handled through `Client::_outputBuffer`?
* Is disconnect cleanup safe?

### Channel / ChannelManager

* Are operators also members?
* Does `removeClient()` remove the client from members, operators, and invited users?
* Does `removeClientFromAllChannels()` prevent dangling `Client*`?
* Do `broadcast()` and `broadcastExcept()` use `appendOutput()`?

### Parser / Commands / Replies

* Does the parser only parse syntax?
* Are business rules handled inside command classes?
* Does `CommandDispatcher` only route commands?
* Do commands use `ReplyBuilder` when possible?
* Do commands avoid direct network I/O?

---

## Keeping branches updated

After another feature branch is merged into `dev`, update your branch:

```bash
git checkout feature/<your-branch>
git fetch origin
git merge origin/dev
```

Then resolve conflicts if needed:

```bash
git status
git add .
git commit
git push
```

For this project, we prefer `merge` over `rebase` because it is simpler and safer for the team.

---

## Merging `dev` into `main`

Only merge `dev` into `main` when:

* the project compiles;
* the current milestone works;
* the team agrees;
* no critical crash is known.

Preferred process:

```txt
dev -> Pull Request -> main
```

`main` must stay stable.

---

## Public interface rule

Public method signatures in `.hpp` files are shared contracts.

Changing a public method may break another teammate’s work.

Therefore:

```txt
Public .hpp signature change = team discussion first.
Private helper method = file owner can add freely.
```

---

## Conflict rule

If a merge conflict happens:

1. Do not panic.
2. Do not randomly delete code.
3. Identify which files conflict.
4. Ask the file owner before modifying their logic.
5. Resolve.
6. Run `make fclean && make`.
7. Commit the resolution.

---

## Team principle

Each teammate owns their part, but the project must stay integrated.

The goal is not only to make the code compile, but to make sure every teammate understands and can defend their own implementation.

````

---
