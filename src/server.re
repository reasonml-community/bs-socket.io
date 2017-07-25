module type M_t = {
  type t 'a;
  let stringify: t 'a => string;
};

type serverT;

type socketT;

module Server (M: M_t) => {

  external _unsafeCreate : 'a => serverT = "socket.io" [@@bs.module];
  let createWithHttp http => _unsafeCreate http;
  /**
   * makeOptions is a simple way to get around the fact that ocaml won't allow you to declare
   * partially defined records (and row polymorphism + ad hoc polymorphism is tricky in ocaml)
   * This allows you to create a JS object with any of the defined properties, allowing to omit
   * any number of them.
   */
  type createOptionsT;
  external makeOptions : pingTimeout::int? =>
                         pingInterval::int? =>
                         maxHttpBufferSize::int? =>
                         transports::list string? =>
                         allowUpgrades::Js.boolean? =>
                         perMessageDeflate::int? =>
                         httpCompression::int? =>
                         cookie::string? =>
                         cookiePath::string? =>
                         wsEngine::string? =>
                         unit =>
                         createOptionsT = "" [@@bs.obj];
  let create (options: createOptionsT) => _unsafeCreate options;
  external createWithPort : int => createOptionsT => serverT = "socket.io" [@@bs.module];
  external serveClient : serverT => Js.boolean => serverT = "serveClient" [@@bs.send];
  external path : serverT => string => serverT = "path" [@@bs.send];
  
  /**
   * This kind of function is annoying because it relies on the type of another module which you might not
   * care about (here https://github.com/socketio/socket.io-adapter), and which is defined by someone else.
   * The only idea I have to typecheck this is to make sure this type is a module type representing an
   * interface, and have the external package also use that module signature. The problem is to keep them in
   * sync. No clue how to do that.
   */
  external adapter : serverT => 'a => serverT = "adapter" [@@bs.send];
  /**
   * This is to support polymorphic args.
   * Super useful trick from
   * http://bloomberg.github.io/bucklescript/Manual.html#_phantom_arguments_and_ad_hoc_polyrmophism
   * See _unsafeEmitT for explanation.
   */
  type kind _ =
    | Func :kind ('a => bool => unit)
    | String :kind string;
  external origins : serverT => (kind 'a) [@bs.ignore] => 'a => serverT = "origins" [@@bs.send];
  external close : serverT => unit = "close" [@@bs.send];
  
  type kind2 _ =
    | Http :kind2 'a
    | Port :kind2 int;
  external attach : serverT => (kind2 'a) [@bs.ignore] => 'a => createOptionsT => serverT = "attach" [@@bs.send];

  external _emit : 'a => string => 'b => unit = "emit" [@@bs.send];

  let emit (server: serverT) (t: M.t 'a) (obj: 'a) :unit => {
    _emit server (M.stringify t) (Json.toValidJson obj)
  };
  /* socketT is the representation of a connection received by the server. It's a pipeline through
   * which one can emit and listen to events.
   */
  module Socket = {
    /* Here 'a means that you can send anything you want, and it'll depend on Bucklescript */
    external _on : socketT => (M.t 'a) [@bs.ignore] => string => ('a => unit) => unit = "on" [@@bs.send];
    let on socket t func => {
      _on socket t (M.stringify t) (fun obj => func (Json.fromValidJson obj))
    };
    let emit (socket: socketT) (t: M.t 'a) (obj: 'a) =>
      _emit socket (M.stringify t) (Json.toValidJson obj);
    type broadcastT;
    external _unsafeBroadcast : socketT => broadcastT = "broadcast" [@@bs.get];
    let broadcast socket str data =>
      _emit (_unsafeBroadcast socket) (M.stringify str) (Json.toValidJson data);
    external id : socketT => string = "id" [@@bs.get];
    external join : socketT => string => ('a => unit) => socketT = "join" [@@bs.send];
    external leave : socketT => string => ('a => unit) => socketT = "leave" [@@bs.send];
    external selectRoom : socketT => string => socketT = "to" [@@bs.send];
    external compress : socketT => Js.boolean => socketT = "compress" [@@bs.send];
    external disconnect : socketT => Js.boolean => socketT = "disconnect" [@@bs.send];
  };
  external _unsafeOnConnect : serverT => string => (socketT => unit) => unit = "on" [@@bs.send];
  let onConnect io cb => _unsafeOnConnect io "connection" cb;
};
