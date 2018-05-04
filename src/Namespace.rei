module Make:
  (M: Common.M_t) =>
  {
    type t;

    /*** Socket.io docs: https://socket.io/docs/server-api/#namespace-name */
    let getName: t => string;

    /*** Socket.io docs: https://socket.io/docs/server-api/#namespace-adapter */
    let getAdapter: t => 'a;

    /*** Socket.io docs: https://socket.io/docs/server-api/#namespace-connected */
    let getConnected: t => Js.t('a);

    /*** Socket.io docs: https://socket.io/docs/server-api/#namespace-clients-callback */
    let clients: (t, ('a, list(string)) => unit) => unit;

    /*** Socket.io docs: https://socket.io/docs/server-api/#namespace-use-fn */
    let use: (t, (BsSocket.Server.socketT, unit => unit) => unit) => unit;

    /*** Socket.io docs: https://socket.io/docs/server-api/#server-sockets */
    let default: BsSocket.Server.serverT => t;

    /*** See example in Socket.io docs: https://socket.io/docs/server-api/#namespace-emit-eventname-args */
    let of_: (BsSocket.Server.serverT, string) => t;

    /*** Socket.io docs: https://socket.io/docs/server-api/#namespace-to-room */
    let to_: (t, string) => t;

    /*** Socket.io docs: https://socket.io/docs/server-api/#namespace-emit-eventname-args */
    let emit: (t, M.t) => unit;

    /*** Socket.io docs: https://socket.io/docs/server-api/#flag-volatile */
    type volatileT;
    let getVolatile: t => volatileT;
    let volatileEmit: (t, M.t) => unit;

    /*** Socket.io docs: https://socket.io/docs/server-api/#flag-local */
    type localT;
    let getLocal: t => localT;
    let localEmit: (t, M.t) => unit;
  };
