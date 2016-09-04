/*
 * vim: set ft=rust:
 * vim: set ft=reason:
 */
/* Same as Server's.
 * `all` is basically a hack which relies on the user listing out all the possible event types that are
 * in use. We internally map over them and hook up event handlers for each one. This allows ocaml's
 * typechecker to prevent you from forgetting to listen for some event.
 */
module type M_t = {type t; let stringify: t => string; let all: list t;};

type serverT;

type socketT;

let module Server (M: M_t) => {
  external _unsafeCreate : 'a => serverT = "socket.io" [@@bs.module];
  let createWithHttp http => _unsafeCreate http;
  /* makeOptions is a simple way to get around the fact that ocaml won't allow you to declare
   * partially defined records (and row polymorphism + ad hoc polymorphism is tricky in ocaml)
   * This allows you to create a JS object with any of the defined properties, allowing to omit
   * any number of them.
   */
  type createOptionsT;
  external makeOptions : pingTimeout::int? =>
                         pingInterval::int? =>
                         maxHttpBufferSize::int? =>
                         transports::list string? =>
                         allowUpgrades::bool? =>
                         perMessageDeflate::int? =>
                         httpCompression::int? =>
                         cookie::string? =>
                         cookiePath::string? =>
                         wsEngine::string? =>
                         unit =>
                         createOptionsT = "" [@@bs.obj];
  let create (options: createOptionsT) => _unsafeCreate options;
  external createWithPort : int => createOptionsT => serverT = "socket.io" [@@bs.module];
  external serveClient : serverT => bool => serverT = "serveClient" [@@bs.send];
  external path : serverT => string => serverT = "path" [@@bs.send];
  /* This kind of function is annoying because it relies on the type of another module which you might not
   * care about (here https://github.com/socketio/socket.io-adapter), and which is defined by someone else.
   * The only idea I have to typecheck this is to make sure this type is a module type representing an
   * interface, and have the external package also use that module signature. The problem is to keep them in
   * sync. No clue how to do that.
   */
  external adapter : serverT => 'a => serverT = "adapter" [@@bs.send];
  /* This is to support polymorphic args.
   * Super useful trick from
   * http://bloomberg.github.io/bucklescript/Manual.html#_phantom_arguments_and_ad_hoc_polyrmophism
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
  external _emit : serverT => string => 'a => unit = "emit" [@@bs.send];
  let emit server t obj => _emit server (M.stringify t) obj;
  /* socketT is the representation of a connection received by the server. It's a pipeline through
   * which one can emit and listen to events.
   */
  let module Socket = {
    /* Here 'a means that you can send anything you want, and it'll depend on Bucklescript */
    external _on : socketT => string => ('a => unit [@bs]) => unit = "on" [@@bs.send];
    let on socket func => ignore @@ List.map (fun t => _on socket (M.stringify t) (func t)) M.all;
    external _emit : socketT => string => 'a => unit = "emit" [@@bs.send];
    let emit socket t obj => _emit socket (M.stringify t) obj;
    type broadcastT;
    external _unsafeBroadcast : socketT => broadcastT = "broadcast" [@@bs.get];
    external _unsafeEmit : broadcastT => string => 'a => unit = "emit" [@@bs.send];
    let broadcast socket str data => _unsafeEmit (_unsafeBroadcast socket) (M.stringify str) data;
    external id : socketT => string = "id" [@@bs.send];
    external join : socketT => string => ('a => unit [@bs]) => socketT = "join" [@@bs.send];
    external leave : socketT => string => ('a => unit [@bs]) => socketT = "leave" [@@bs.send];
    external selectRoom : socketT => string => socketT = "to" [@@bs.send];
    external compress : socketT => bool => socketT = "compress" [@@bs.send];
    external disconnect : socketT => bool => socketT = "disconnect" [@@bs.send];
  };
  external _unsafeOnConnect : serverT => string => (socketT => unit [@bs]) => unit = "on" [@@bs.send];
  let onConnect io cb => _unsafeOnConnect io "connection" cb;
};
