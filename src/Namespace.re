type t;

/* Socket.io docs: https://socket.io/docs/server-api/#namespace-name */
[@bs.get] external getName: t => string = "name";

/* Socket.io docs: https://socket.io/docs/server-api/#namespace-adapter */
[@bs.get] external getAdapter: t => 'a = "adapter";

/* Socket.io docs: https://socket.io/docs/server-api/#namespace-connected */
[@bs.get] external getConnected: t => Js.t('a) = "connected";

/* Socket.io docs: https://socket.io/docs/server-api/#namespace-clients-callback */
[@bs.send] external clients: (t, ('a, list(string)) => unit) => unit = "clients";

/* Socket.io docs: https://socket.io/docs/server-api/#namespace-use-fn */
[@bs.send] external use: (t, (Socket.socketT, unit => unit) => unit) => unit = "use";

/* Socket.io docs: https://socket.io/docs/server-api/#server-sockets */
[@bs.send] external default: Server.serverT => t = "sockets";

/* See example in Socket.io docs: https://socket.io/docs/server-api/#namespace-emit-eventname-args */
[@bs.send] external of_: (Server.serverT, string) => t = "of";

/* Socket.io docs: https://socket.io/docs/server-api/#namespace-to-room */
[@bs.send] external to_: (t, string) => t = "to";

/* Socket.io docs: https://socket.io/docs/server-api/#namespace-emit-eventname-args */
[@bs.send] external emit: (t, string, 'a) => unit = "emit";

/* Socket.io docs: https://socket.io/docs/server-api/#flag-volatile */
type volatileT;
[@bs.get] external getVolatile: t => volatileT = "volatile";
[@bs.send] external emitVolatile: (volatileT, string, 'a) => unit = "emit";

/* Socket.io docs: https://socket.io/docs/server-api/#flag-local */
type localT;
[@bs.get] external getLocal: t => localT = "local";
[@bs.send] external emitLocal: (localT, string, 'a) => unit = "emit";
