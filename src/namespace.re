module Namespace (M: Common.M_t) => {
  type t;
  external getName : t => string = "name" [@@bs.get];
  external getAdapter : t => 'a = "adapter" [@@bs.get];
  /* Returns a JS object with socket IDs as keys. */
  external getConnected : t => Js.t 'a = "connected" [@@bs.get];
  external clients : t => ('a => list string => unit) => unit =
    "clients" [@@bs.send];
  external use : t => (Server.socketT => (unit => unit) => unit) => unit =
    "use" [@@bs.send];
  external default : Server.serverT => t = "sockets" [@@bs.send];
  /* This is "of" in socket.io. */
  external of_ : Server.serverT => string => t = "of" [@@bs.send];
  /* This is "to" in socket.io or the "in" (they're synonyms apparently) */
  external to_ : t => string => t = "to" [@@bs.send];
  external _emit : t => string => 'a => unit = "emit" [@@bs.send];

  /** */
  let emit (server: t) (t: M.t 'a) (obj: 'a) :unit =>
    _emit server (M.stringify t) (Json.toValidJson obj);

  /** */
  type volatileT;
  external getVolatile : t => volatileT = "volatile" [@@bs.get];
  external _volatileEmit : volatileT => string => 'a => unit =
    "emit" [@@bs.send];
  let volatileEmit (server: t) (t: M.t 'a) (obj: 'a) :unit =>
    _volatileEmit (getVolatile server) (M.stringify t) (Json.toValidJson obj);

  /** */
  type localT;
  external getLocal : t => localT = "local" [@@bs.get];
  external _localEmit : localT => string => 'a => unit = "emit" [@@bs.send];
  let localEmit (server: t) (t: M.t 'a) (obj: 'a) :unit =>
    _localEmit (getLocal server) (M.stringify t) (Json.toValidJson obj);
};
