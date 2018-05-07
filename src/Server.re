type serverT;

type socketT;

type room = string;

module Make = (M: Common.M_t) => {
  [@bs.module] external create : unit => serverT = "socket.io";
  [@bs.module] external createWithHttp : 'a => serverT = "socket.io";

  /***
   * makeOptions is a simple way to get around the fact that ocaml won't allow you to declare
   * partially defined records (and row polymorphism + ad hoc polymorphism is tricky in ocaml)
   * This allows you to create a JS object with any of the defined properties, allowing to omit
   * any number of them.
   */
  type createOptionsT;
  [@bs.obj]
  external makeOptions :
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
    createOptionsT =
    "";
  [@bs.module]
  external createWithOptions : createOptionsT => serverT = "socket.io";
  [@bs.module]
  external createWithHttpAndOption : ('a, createOptionsT) => serverT =
    "socket.io";
  [@bs.module]
  external createWithPort : (int, createOptionsT) => serverT = "socket.io";

  /*** */
  [@bs.send] external serveClient : (serverT, bool) => serverT = "serveClient";

  /*** */
  [@bs.send] external path : (serverT, string) => serverT = "path";

  /***
   * This kind of function is annoying because it relies on the type of another module which you might not
   * care about (here https://github.com/socketio/socket.io-adapter), and which is defined by someone else.
   * The only idea I have to typecheck this is to make sure this type is a module type representing an
   * interface, and have the external package also use that module signature. The problem is to keep them in
   * sync. No clue how to do that.
   */
  [@bs.send] external adapter : (serverT, 'a) => serverT = "adapter";

  /*** */
  [@bs.send] external origins : (serverT, string) => serverT = "origins";
  [@bs.send]
  external originsWithFunc : (serverT, ('a, bool) => unit) => serverT =
    "origins";

  /*** */
  [@bs.send] external close : serverT => unit = "close";

  /*** This is the same as "server.listen". */
  [@bs.send]
  external attach : (serverT, 'a, createOptionsT) => serverT = "attach";
  [@bs.send]
  external attachWithPort : (serverT, int, createOptionsT) => serverT =
    "attach";

  /*** */
  [@bs.send] external _emit : ('a, string, 'b) => unit = "emit";

  /***
   * socketT is the representation of a connection received by the server.
   * It's a pipeline through which one can emit and listen to events.
   */
  module Socket = {
    [@bs.get] external getId : socketT => room = "id";
    [@bs.get] external getRooms : socketT => Js.t('a) = "rooms";
    [@bs.get] external getHandshake : socketT => Js.t('a) = "handshake";
    /* Here 'a means that you can send anything you want, and it'll depend on
       Bucklescript */
    [@bs.send] external _on : (socketT, string, M.t => unit) => unit = "on";
    let on = (socket, func) =>
      _on(socket, "message", obj => func(Json.fromValidJson(obj)));

    /*** */
    let emit = (socket: socketT, obj: M.t) =>
      _emit(socket, "message", Json.toValidJson(obj));

    /*** */
    type broadcastT;
    [@bs.get]
    external _unsafeGetBroadcast : socketT => broadcastT = "broadcast";
    let broadcast = (socket, data: M.t) =>
      _emit(_unsafeGetBroadcast(socket), "message", Json.toValidJson(data));

    /*** */
    [@bs.send]
    external join : (socketT, string, 'a => unit) => socketT = "join";
    let join = (socket, room, onError) =>
      join(socket, room, err => onError(~err));
    [@bs.send]
    external leave : (socketT, string, 'a => unit) => socketT = "leave";
    let leave = (socket, room, onError) =>
      leave(socket, room, err => onError(~err));
    [@bs.send] external to_ : (socketT, string) => socketT = "to";
    [@bs.send] external compress : (socketT, bool) => socketT = "compress";
    [@bs.send] external disconnect : (socketT, bool) => socketT = "disconnect";
    [@bs.send]
    external use : (socketT, ('a, unit => unit) => unit) => unit = "use";
    let use = (socket, f) =>
      use(socket, (packet, next) => f(~packet, ~next));

    /*** */
    [@bs.send]
    external _once : (socketT, string, M.t => unit) => unit = "once";
    let once = (socket, func) =>
      _once(socket, "message", obj => func(Json.fromValidJson(obj)));

    /*** Volatile */
    type volatileT;
    [@bs.get] external getVolatile : socketT => volatileT = "volatile";
    [@bs.send]
    external _volatileEmit : (volatileT, string, 'a) => unit = "emit";
    let volatileEmit = (server: socketT, obj: M.t) : unit =>
      _volatileEmit(getVolatile(server), "message", Json.toValidJson(obj));
    let onDisconnect = (socket, cb) =>
      _on(socket, "disconnect", (_) => cb());
  };
  [@bs.send]
  external _unsafeOnConnect : (serverT, string, socketT => unit) => unit =
    "on";
  let onConnect = (io, cb) => _unsafeOnConnect(io, "connection", cb);
};
