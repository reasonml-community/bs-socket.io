bs-socket
---

Simple bindings to [socket.io](https://socket.io/).

To build everything run `npm run build` and to run the demo run `npm run run` and go to `localhost:3000` on two tabs. You can send messages back and forth!

# Documentation

Everything lives under the namespace BsSocket.

```reason
module MyServer = BsSocket.Server.Make({
  type t = ...;
});
```

See `example/` folder for full usage.

The API reflects socket.io's API. Generally, e.g. JavaScript's `socket.emit("bla", 10)` becomes `Server.emit(socket, "bla", 10)` in Reason.

To create a server/client/namespace, use `Server.Make`, `Client.Make` and `Namespace.Make` respectively. These functors take a module that contains 2 things: a type called `t` and a function called `stringify`. This allows ocaml to typecheck the messages that you send back and forth between the server and the client. ie: you can only listen to/send messages that the server can listen to/send too.

## Couple Gotchas

There are a couple differences between the JS API and this one. We'll refer to the supposed module you've created from the `Server.Make` functor as `MyServer`. Same for `Namespace` and `Client`.

- `MyServer.emit` is different from `MyServer.Socket.emit`. The former emits to all connected peers while the latter emits to the given peer.
- Instead of `io.sockets` there is `MyNamespace.default(io)` which does the same thing.
- Instead of `io.of` there is `MyNamespace.of_(io)` which does the same thing. (`of` is a Reason keyword)
- All functions that are overloaded have different names depending on what you're passing. There's `MyServer.create` but also `MyServer.createWithHttp` (see example) among others.
