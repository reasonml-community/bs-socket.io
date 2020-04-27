module Socket = {
  type t;

  // In reality this is replicating a JS error object
  type error = {
    name: string,
    message: string,
  };

  /* EVENT HANDLING */

  // socket.on("connect", () => { ... })
  [@bs.send]
  external onConnect: (t, [@bs.as "connect"] _, unit => unit) => unit = "on";

  // socket.on("reconnect", (attemptNumber) => { ... })
  [@bs.send]
  external onReconnect: (t, [@bs.as "reconnect"] _, int => unit) => unit =
    "on";

  // socket.on("connect_error", (err) => { ... })
  [@bs.send]
  external onConnectError:
    (t, [@bs.as "connect_error"] _, error => unit) => unit =
    "on";

  // socket.on("reconnecting", (attemptNumber) => { ... })
  [@bs.send]
  external onReconnecting: (t, [@bs.as "reconnecting"] _, int => unit) => unit =
    "on";

  // socket.on("reconnect_failed", () => { ... })
  [@bs.send]
  external onReconnectFailed:
    (t, [@bs.as "reconnect_failed"] _, unit => unit) => unit =
    "on";

  // socket.on("reconnect_error", (err) => { ... })
  [@bs.send]
  external onReconnectError:
    (t, [@bs.as "reconnect_error"] _, error => unit) => unit =
    "on";

  // socket.on("some_random_event", arg1, arg2, arg3,..., (data) => { ... })
  [@bs.send]
  external onEvent: (t, [@bs.as "event"] _, string, Js.Json.t => unit) => unit =
    "on";

  // same as onEvent, but with 2 arguments in the result callback
  [@bs.send]
  external onEvent2:
    (t, [@bs.as "event"] _, string, (Js.Json.t, Js.Json.t) => unit) => unit =
    "on";

  // same as onEvent, but with 3 arguments in the result callback
  [@bs.send]
  external onEvent3:
    (
      t,
      [@bs.as "event"] _,
      string,
      (Js.Json.t, Js.Json.t, Js.Json.t) => unit
    ) =>
    unit =
    "on";

  [@bs.send]
  external onError: (t, [@bs.as "error"] _, error => unit) => unit = "on";

  [@bs.send]
  external onPing: (t, [@bs.as "ping"] _, unit => unit) => unit = "on";

  // socket.on("pong", (latency) => { ... });
  [@bs.send]
  external onPong: (t, [@bs.as "pong"] _, int => unit) => unit = "on";

  [@bs.send]
  external onDisconnect: (t, [@bs.as "disconnect"] _) => unit = "on";

  /* METHODS */
  [@bs.send] external connect: t => unit = "connect";

  [@bs.send] external close: t => unit = "close";
  [@bs.send] external emit: (t, string) => unit = "emit";
};

module Client = {
  [@bs.module "socket.io-client"] [@bs.new]
  external create: unit => Socket.t = "io";
  [@bs.module "socket.io-client"] [@bs.new]
  external createWithUrl: string => Socket.t = "io";
};


