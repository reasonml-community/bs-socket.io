module Make:
  (M: Common.M_t) =>
  {
    type t;

    /*** Socket.io docs: https://socket.io/docs/client-api/#io-url-options */
    let create: unit => t;

    /*** Same as the server-side `emit`, doesn't take a message name.
         The recommended payload type to send is a variant wSocket.io docs: https://socket.io/docs/client-api/#socket-emit-eventname-args-ack */
    let emit: (t, M.t) => unit;

    /*** Same ideas as explained above.
         Socket.io docs: https://socket.io/docs/client-api/#socket-on-eventname-callback */
    let on: (t, M.t => unit) => unit;
  };
