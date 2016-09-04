/*
 * vim: set ft=rust:
 * vim: set ft=reason:
 */
module type M_t = {type t; let stringify: t => string;};

let module Client (M: M_t) => {
  type t;
  external create : unit => t = "io" [@@bs.new];
  external _emit : t => string => 'a => unit = "emit" [@@bs.send];
  let emit socket t typ => _emit socket (M.stringify t) typ;
  external _on : t => string => ('a => unit [@bs]) => unit = "on" [@@bs.send];
  let on socket t cb => _on socket (M.stringify t) cb;
};
