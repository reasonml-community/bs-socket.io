bs-socket
---

Simple bindings to [socket.io](https://socket.io/).

To build everything run `npm run build` and to run the demo run `npm run run` and go to `localhost:3000` on two tabs. You can send messages back and forth!

# Documentation

Everything lives under the namespace BsSocket.

```reason
module Messages = {
  type username = string;
  type clientToServer = 
  | Login(username);
  type serverToClient = 
  | LoginSuccessful(bool);
};
module MyServer = BsSocket.Server.Make(Messages);
let io = MyServer.create();
```

See `example/` folder for more.

The API reflects socket.io's API. Generally, e.g. JavaScript's `socket.emit("bla", 10)` becomes `Server.emit(socket, "bla", 10)` in Reason.

To create a server/client/namespace, use `Server.Make`, `Client.Make` and `Namespace.Make` respectively. These functors take a module that contains 2 things: a type called `clientToServer` and a type called `serverToClient`, which define the type of the message that the client will send to the server and vice versa.  A common pattern is to use the same message type for `clientToServer` and `serverToClient` in the following way:

```
module Messages = {
  type t = | ...;
  type clientToServer = t;
  type serverToClient = t;
};
```

## Conceptual difference between socket.io and bs-socket.io

Whereas in socket.io, emitting a message requires a string as the
first argument as a way to tag what type of message you're sending, in
bs-socket.io, the intended usage is that there is only one type of
message and therefore there is no string tag required.  This is so
that the function that handles messages from the other side of the
socket, i.e. `MyClient.on` and `MyServer.on`, can leverage Reason's
exhaustive pattern matching on variant types, which can help ensure
that they are handling all potential message variants that they could
possibly be sent.

As a concrete example, the following socket.io and bs-socket.io pseudo-code
would be analogous to each other.

```
// socket.io 
// client
socket.emit('login', { username: 'user2157' });
socket.emit('chat message', 'hello');

// server
socket.on('login', msg => ...);
socket.on('chat message', msg => ...);
```

```
// bs-socket.io 
// client
MyClient.emit(Login("2157"));
MyClient.emit(ChatMessage("hello"));

// server
MyServer.on(socket, msg => 
  switch(msg) {
  | Login(username) => ...
  | ChatMessage(msg) => ...
  );
```

If you later extended the type of `clientToServer` to have another case, i.e.

```
type username = string;
type clientToServer = 
  | Login(username)
  | ChatMessage(string)
  | Logout(username);
```

Then the previous implementation of `MyServer.on` would no longer
compile with the reason that you haven't handled the case where the
message is `Logout`.

## Couple Gotchas

There are a couple differences between the JS API and this one. We'll refer to the supposed module you've created from the `Server.Make` functor as `MyServer`. Same for `Namespace` and `Client`.

- `MyServer.emit` is different from `MyServer.Socket.emit`. The former emits to all connected peers while the latter emits to the given peer.
- Instead of `io.sockets` there is `MyNamespace.default(io)` which does the same thing.
- Instead of `io.of` there is `MyNamespace.of_(io)` which does the same thing. (`of` is a Reason keyword)
- All functions that are overloaded have different names depending on what you're passing. There's `MyServer.create` but also `MyServer.createWithHttp` (see example) among others.
