/* Socket.io docs: https://socket.io/docs/server-api/#socket */
type broadcastT;
type socketT;

/* Small difference here between socket.io and bs-socket. You don't need to pass a string
   for the event name, it's better to simply use a variant as the representation of the event
   type and payload.

   Socket.io docs: https://socket.io/docs/server-api/#socket-on-eventname-callback */
[@bs.send] external on: (socketT, string, 'data => unit) => unit = "on";
let on = (socket, message, cb) => {
  on(socket, message, data => cb(. Serializer.fromValidJson(data)));
};

/* Special function listening for the message 'disconnect'. Same behavior as `on`. */
[@bs.send]
external onDisconnect: (socketT, [@bs.as "disconnect"] _, string => unit) => unit = "on";

/* Special function listening for the message 'connection'. Same behavior as `on`. */
[@bs.send] external onConnect: (socketT, [@bs.as "connection"] _, unit => unit) => unit = "on";

/* Same difference as stated above.
   Socket.io docs: https://socket.io/docs/server-api/#socket-emit-eventname-args-ack */
[@bs.send] external emit: (socketT, string, 'data) => unit = "emit";
let emit = (socket, message, data) => {
  emit(socket, message, Serializer.toValidJson(data));
};

/* Socket.io docs: https://socket.io/docs/server-api/#socket-compress-value */
[@bs.send] external compress: (socketT, bool) => socketT = "compress";

/* Socket.io docs: https://socket.io/docs/server-api/#socket-disconnect-close */
[@bs.send] external disconnect: (socketT, bool) => socketT = "disconnect";

/* Socket.io docs: https://socket.io/docs/server-api/#socket-use-fn */
[@bs.send] external use: (socketT, ('a, ~next: unit => unit) => unit) => unit = "use";

/* Socket.io docs: https://socket.io/docs/server-api/#socket-once-eventname-listener */
[@bs.send] external once: (socketT, string, 'a => unit) => unit = "once";
let once = (socket, message, cb) => {
  once(socket, message, data => cb(. Serializer.fromValidJson(data)));
};

/* Socket.io docs: https://socket.io/docs/server-api/#flag-volatile */
type volatileT;
[@bs.get] external getVolatile: socketT => volatileT = "volatile";
[@bs.send] external emitVolatile: (volatileT, string, string) => unit = "emit";
let emitVolatile = (socket, message, data) => {
  emitVolatile(socket, message, Serializer.toValidJson(data));
};

type room = string;
/* Socket.io docs: https://socket.io/docs/server-api/#socket-id
   A socket's unique id can be thought of as a room that has only that socket. */
[@bs.get] external getId: socketT => room = "id";

/* Socket.io docs: https://socket.io/docs/server-api/#socket-rooms */
[@bs.get] external getRooms: socketT => Js.t('a) = "rooms";

/* Socket.io docs: https://socket.io/docs/server-api/#socket-handshake */
[@bs.get] external getHandshake: socketT => Js.t('a) = "handshake";

/* Socket.io docs: https://socket.io/docs/server-api/#socket-join-room-callback */
[@bs.send] external join: (socketT, room) => socketT = "join";

/* Socket.io docs: https://socket.io/docs/server-api/#socket-leave-room-callback */
[@bs.send] external leave: (socketT, room) => socketT = "leave";

/* Socket.io docs: https://socket.io/docs/server-api/#socket-to-room */
[@bs.send] external to_: (socketT, room) => socketT = "to";
