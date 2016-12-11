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

module Server (M: M_t) => {
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

  /**
   * This is used because variants (in Bucklescript) are not naturally serializable.
   * Bucklescript adds a `tag` property to the array (array is the representation of a variant in JS) which
   * doesn't get carried over in JSON. For simplicity, the data is wrapped into an extra array where the
   * first element is the tag or -1, and the 2nd element is the actual data. When receiving the packet we
   * attach the tag back onto the deserialized object if it's not -1.
   *
   * _unsafeEmitT is used because if we typed the function `'a => string => 'b => unit`, the inference
   * algorithm will use the first usage as a way to resolve the type, and will not allow for any _other_ 'a.
   * Say we do `_unsafeEmit 10 "asd" 1.5`, the typechecker will infer the type `int => string => float`.
   * If we do right below: `_unsafeEmit "clearly a string" "asd" 1.5` the type checker will emit an error
   * because it has resolved the first argument to be of type `int` and now we're passing a string.
   * This problem can be solved thanks to GADT, which is what _unsafeEmitT does.
   *
   * Say we made a new type:
   *
   * type gadtExampleT 'a =
   *   | Int : gadtExampleT int
   *   | String : gadtExampleT string;
   *
   * We now write the type of this function as `gadtExampleT 'a => 'a => string => 'b => unit`.
   * On the first usage, we can do `_unsafeEmit Int 10 "bla" 1.5` and the type will be inferred as
   * `gadtExampleT 'a => 'a => string => float => unit`. The first two arguments stay the same because the
   * typechecker doesn't need to infer the arguments as the first indicates what the 2nd, and the first isn't
   * an "unbound" type (like 'a is).
   * We can then safely do `_unsafeEmit String "clearly a string" "bla" 1.5` and the typechecker will be
   * happy.
   */
  type _unsafeEmitT 'a =
    | ServerKind :_unsafeEmitT serverT
    | SocketKind :_unsafeEmitT socketT
    /* This type's intentionally left open because it's defined inside the Socket module.
     * This is an annoying problem...
     */
    | BroadcastKind :_unsafeEmitT 'a;
  /* We swallow the first argument because we actually don't care. */
  let _unsafeEmit: type a. _unsafeEmitT a => a => string => 'b => unit =
    fun a => [%bs.raw
      {|
      function(server, stringType, obj) {
        server.emit(stringType, [obj.hasOwnProperty("tag") ? obj.tag : -1, obj]);
      }
    |}
    ];
  external _emit : serverT => string => 'a => unit = "emit" [@@bs.send];
  let emit (server: serverT) (t: M.t) (obj: 'a) :unit => {
    let stringType = M.stringify t;
    _unsafeEmit ServerKind server stringType obj
  };
  /* socketT is the representation of a connection received by the server. It's a pipeline through
   * which one can emit and listen to events.
   */
  module Socket = {
    /* Here 'a means that you can send anything you want, and it'll depend on Bucklescript */
    external _on : socketT => string => ('a => unit [@bs]) => unit = "on" [@@bs.send];
    let on_not_ready_yet socket func =>
      List.iter (fun t => _on socket (M.stringify t) (func t)) M.all;
    let on socket t func => {
      let assumeObjWithFirstElemTag: 'a => 'b = [%bs.raw
        {|
        function(obj) {
          var ret = obj[1];
          if (obj[0] != -1) {
            ret.tag = obj[0];
          }
          return ret;
        }
      |}
      ];
      _on socket (M.stringify t) (fun obj => func (assumeObjWithFirstElemTag obj))
    };
    /* external _emit : socketT => string => 'a => unit = "emit" [@@bs.send]; */
    let emit (socket: socketT) (t: M.t) (obj: 'a) =>
      _unsafeEmit SocketKind socket (M.stringify t) obj;
    type broadcastT;
    external _unsafeBroadcast : socketT => broadcastT = "broadcast" [@@bs.get];
    /* external _unsafeEmit : broadcastT => string => 'a => unit = "emit" [@@bs.send]; */
    let broadcast socket str data =>
      _unsafeEmit BroadcastKind (_unsafeBroadcast socket) (M.stringify str) data;
    external id : socketT => string = "id" [@@bs.get];
    external join : socketT => string => ('a => unit [@bs]) => socketT = "join" [@@bs.send];
    external leave : socketT => string => ('a => unit [@bs]) => socketT = "leave" [@@bs.send];
    external selectRoom : socketT => string => socketT = "to" [@@bs.send];
    external compress : socketT => bool => socketT = "compress" [@@bs.send];
    external disconnect : socketT => bool => socketT = "disconnect" [@@bs.send];
  };
  external _unsafeOnConnect : serverT => string => (socketT => unit [@bs]) => unit = "on" [@@bs.send];
  let onConnect io cb => _unsafeOnConnect io "connection" cb;
};
