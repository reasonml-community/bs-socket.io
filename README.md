ReSocket.io
---

This is a dead simple wrapper for socket.io. It supports most of the API (don't hesitate to open issues to get more if needed).

To build everything run `npm run all` and to run the demo run `npm run run` and go to `localhost:3000` on two tabs. You can send messages back and forth!

# Couple Gotchas
There are a couple differences between the JS API and this one.

- There are no methods, just functions that take as first parameter the object on which they interact. ie: `socket.emit("bla", 10)` is `Server.emit socket "bla" 10` in Reason.
- `Server.Server` `Client.Client` and `Namespace.Namespace` are functors, meaning that they'll take a module that contains 2 things: a type called `t`, a function called `stringify`. This allows ocaml to typecheck the messages that you send back and forth between the server and the client. ie: you can only listen to / send messages that the server can listen to / send too.
- `Server.emit` is different from `Server.Socket.emit`. The former emits to all connected peers while the latter emits to the given peer.
- Instead of `io.sockets` there is `Namespace.default io` which does the same thing.
- Instead of `io.of` there is `Namespace.of_ io` which does the same thing. (`of` is a Reason keyword)
- All functions that are overloaded have different names depending on what you're passing. There's `Server.create` but also `Server.createWithHtpp` (see example) among others.
