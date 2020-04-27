type serverT = Socket.socketT;

/* Socket.io docs: https://socket.io/docs/server-api/#server */
[@bs.module] external create: unit => serverT = "socket.io";
/* These functions takes an instance of `http` from NodeJS stdlib, which you can get by simply
   doing `require('http')`.

   Socket.io docs: https://socket.io/docs/server-api/#server */
[@bs.module] external createWithHttp: 'a => serverT = "socket.io";

/*
 * makeOptions is a simple way to get around the fact that ocaml won't allow you to declare
 * partially defined records (and row polymorphism + ad hoc polymorphism is tricky in ocaml)
 * This allows you to create a JS object with any of the defined properties, allowing to omit
 * any number of them.
 */
type optionsT;
[@bs.obj]
external makeOptions:
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
  optionsT;

[@bs.module] external createWithHttpAndOption: ('a, optionsT) => serverT = "socket.io";
[@bs.module] external createWithOptions: optionsT => serverT = "socket.io";
[@bs.module] external createWithPort: (int, optionsT) => serverT = "socket.io";

/* Socket.io docs: https://socket.io/docs/server-api/#server-serveclient-value */
[@bs.send] external serveClient: (serverT, bool) => serverT = "serveClient";

/* Socket.io docs: https://socket.io/docs/server-api/#server-path-value */
[@bs.send] external path: (serverT, string) => serverT = "path";

/* Socket.io docs: https://socket.io/docs/server-api/#server-adapter-value */
[@bs.send] external adapter: (serverT, 'a) => serverT = "adapter";

/* Socket.io docs: https://socket.io/docs/server-api/#server-origins-value */
[@bs.send] external origins: (serverT, string) => serverT = "origins";

/* Socket.io docs: https://socket.io/docs/server-api/#server-origins-fn */
[@bs.send] external originsWithFunc: (serverT, ('a, bool) => unit) => serverT = "origins";

/* Socket.io docs: https://socket.io/docs/server-api/#server-path-value */
[@bs.send] external close: serverT => unit = "close";

/* Socket.io docs: https://socket.io/docs/server-api/#server-attach-httpserver-options */
[@bs.send] external attach: (serverT, 'a, optionsT) => serverT = "attach";

/* Socket.io docs: https://socket.io/docs/server-api/#server-attach-port-options */
[@bs.send] external attachWithPort: (serverT, int, optionsT) => serverT = "attach";

/* Socket.io docs: https://socket.io/docs/server-api/#Event-‘connection’ */
[@bs.send]
external onConnect: (serverT, [@bs.as "connection"] _, Socket.socketT => unit) => unit = "on";

/* Socket.io docs: https://socket.io/docs/server-api/#Flag-‘broadcast’ */
[@bs.send] [@bs.scope "broadcast"]
external emitBroadcast: (Socket.socketT, string, 'a) => unit = "emit";
let emitBroadcast = (socket, message, data) => {
  emitBroadcast(socket, message, Serializer.toValidJson(data));
};
