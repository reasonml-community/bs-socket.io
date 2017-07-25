module type M_t = {
  type t 'a;
  let stringify: t 'a => string;
};

let module Client (M: M_t) => {
  type t;
  external create : unit => t = "io" [@@bs.new];
  external _emit : t => string => 'a => unit = "emit" [@@bs.send];
  let emit socket (message : M.t 'a) (obj : 'a) => {
    _emit socket (M.stringify message) (Json.toValidJson obj);
  };
  external _on : t => string => ('a => unit) => unit = "on" [@@bs.send];
  let on socket (message : M.t 'a) (func : 'a => unit) => {
    _on socket (M.stringify message) (fun obj => func (Json.fromValidJson obj));
  };
};
