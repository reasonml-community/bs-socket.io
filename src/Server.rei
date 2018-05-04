type serverT;

type socketT;

/* This is a really thin wrapper around socket.io. I recommend checking their docs for explanation
   of how this works. */
module Make:
  (M: Common.M_t) =>
  {

    /*** These functions takes an instance of `http` from NodeJS stdlib, which you can get by simply
         doing `require('http')`.

         Socket.io docs: https://socket.io/docs/server-api/#server */
    let createWithHttp: 'httpServer => serverT;
    type createOptionsT;
    let makeOptions:
      (
        ~pingTimeout: int=?,
        ~pingInterval: int=?,
        ~maxHttpBufferSize: int=?,
        ~transports: list(string)=?,
        ~allowUpgrades: bool=?,
        ~perMessageDeflate: int=?,
        ~httpCompression: int=?,
        ~cookie: string=?,
        ~cookiePath: string=?,
        ~wsEngine: string=?,
        unit
      ) =>
      createOptionsT;
    let createWithHttpAndOption: ('httpServer, createOptionsT) => serverT;

    /*** Socket.io docs: https://socket.io/docs/server-api/#server */
    let create: unit => serverT;
    let createWithOptions: createOptionsT => serverT;
    let createWithPort: (int, createOptionsT) => serverT;

    /*** Socket.io docs: https://socket.io/docs/server-api/#server-serveclient-value */
    let serveClient: (serverT, bool) => serverT;

    /*** Socket.io docs: https://socket.io/docs/server-api/#server-path-value */
    let path: (serverT, string) => serverT;

    /*** Socket.io docs: https://socket.io/docs/server-api/#server-adapter-value */
    let adapter: (serverT, 'a) => serverT;

    /*** Socket.io docs: https://socket.io/docs/server-api/#server-origins-value */
    let origins: (serverT, string) => serverT;

    /*** Socket.io docs: https://socket.io/docs/server-api/#server-origins-fn */
    let originsWithFunc: (serverT, ('a, bool) => unit) => serverT;

    /*** Socket.io docs: https://socket.io/docs/server-api/#server-path-value */
    let close: serverT => unit;

    /*** Socket.io docs: https://socket.io/docs/server-api/#server-attach-httpserver-options */
    let attach: (serverT, 'a, createOptionsT) => serverT;

    /*** Socket.io docs: https://socket.io/docs/server-api/#server-attach-port-options */
    let attachWithPort: (serverT, int, createOptionsT) => serverT;

    /*** Socket.io docs: https://socket.io/docs/server-api/#socket */
    module Socket: {

      /*** Socket.io docs: https://socket.io/docs/server-api/#socket-id */
      let getId: socketT => string;

      /*** Socket.io docs: https://socket.io/docs/server-api/#socket-rooms */
      let getRooms: socketT => Js.t('a);

      /*** Socket.io docs: https://socket.io/docs/server-api/#socket-handshake */
      let getHandshake: socketT => Js.t('a);

      /*** Small difference here between socket.io and bs-socket. You don't need to pass a string
           for the event name, it's better to simply use a variant as the representation of the event
           type and payload.

           Socket.io docs: https://socket.io/docs/server-api/#socket-on-eventname-callback */
      let on: (socketT, 'a => unit) => unit;

      /*** Same difference as stated above.
           Socket.io docs: https://socket.io/docs/server-api/#socket-emit-eventname-args-ack */
      let emit: (socketT, M.t) => unit;

      /*** Socket.io docs: https://socket.io/docs/server-api/#flag-broadcast */
      type broadcastT;
      let broadcast: (socketT, M.t) => unit;

      /*** Socket.io docs: https://socket.io/docs/server-api/#socket-join-room-callback */
      let join: (socketT, string, 'a => unit) => socketT;

      /*** Socket.io docs: https://socket.io/docs/server-api/#socket-leave-room-callback */
      let leave: (socketT, string, 'a => unit) => socketT;

      /*** Socket.io docs: https://socket.io/docs/server-api/#socket-to-room */
      let to_: (socketT, string) => socketT;

      /*** Socket.io docs: https://socket.io/docs/server-api/#socket-compress-value */
      let compress: (socketT, bool) => socketT;

      /*** Socket.io docs: https://socket.io/docs/server-api/#socket-disconnect-close */
      let disconnect: (socketT, bool) => socketT;

      /*** Socket.io docs: https://socket.io/docs/server-api/#socket-use-fn */
      let use: (socketT, ('a, unit => unit) => unit) => unit;

      /*** Socket.io docs: https://socket.io/docs/server-api/#socket-once-eventname-listener */
      let once: (socketT, 'a => unit) => unit;

      /*** Socket.io docs: https://socket.io/docs/server-api/#flag-volatile */
      type volatileT;
      let getVolatile: socketT => volatileT;
      let volatileEmit: (socketT, M.t) => unit;

      /*** Special function listening for the message 'disconnect'. Same behavior as `on`. */
      let onDisconnect: (socketT, unit => unit) => unit;
    };

    /*** Special function listening for the message 'connection'. Same behavior as `on`. */
    let onConnect: (serverT, socketT => unit) => unit;
  };
