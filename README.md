ReSocket.io
---

This is a dead simple wrapper for socket.io. It supports most of the API (don't hesitate to open issues to get more if needed).

To build everything run `npm run all` and to run the demo run `node exampleserver.js` and go to `localhost:3000` on two tabs. You can send messages back and forth!

# Couple Gotchas
There are a couple differences between the JS API and this one.

- There are no methods, just functions that take as first parameter the object on which they interact. ie: `socket.emit("bla", 10)` is `Server.emit socket "bla" 10` in Reason.
- `Server.Server` and `Client.Client` are functors, meaning that they'll take a module that contains two things: a type called `t` and a function called `stringify`. This allows ocaml to typecheck the messages that you send back and forth between the server and the client. ie: you can only listen to / send messages that the server can listen to / send too.
- Because of the above, `Socket.on` works a bit differently than socket.io's. Its type signature is `socketT => (commonDispatchEventT => 'a => unit)` where `commonDispatchEventT` is a type that you define inside the module that you pass to the functors `Server.Server` or `Client.Client`. This allws the type checker to make sure you handle all possible types of events.
- `Server.emit` is different from `Server.Socket.emit`. The former emits to all connected peers while the latter emits to the given peer.
- Instead of `io.sockets` there is `Namespace.default io` which does the same thing.
- Instead of `io.of` there is `Namespace.from io` which does the same thing. (`of` is a Reason keyword)
- Instead of `io.use` there is `Namespace.use io`.
- All functions that are overloaded use the same trick with variants. `io.attach` is defined on an http client AND a port, so in Reason you tell the function which one to use: `Server.attach io Http x` or `Server.attach io Port x`.
