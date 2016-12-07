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

let module Client (M: M_t) => {
  type t;
  external create : unit => t = "io" [@@bs.new];
  external _emit : t => string => 'a => unit = "emit" [@@bs.send];
  let emit socket t typ => _emit socket (M.stringify t) typ;
  external _on : t => string => ('a => unit [@bs]) => unit = "on" [@@bs.send];
  let on_not_ready_yet socket func => List.map (fun t => _on socket (M.stringify t) (func t)) M.all;
  let on socket t func => _on socket (M.stringify t) func;
};
