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
  let emit socket t obj => {
    let stringType = M.stringify t;
    /*let internalSend : t => string => 'a => unit = [%bs.raw {|
      function(socket, stringType, obj) {
        socket.emit(stringType, [obj.hasOwnProperty("tag") ? obj.tag : -1, obj]);
      }
    |}];*/
    _emit socket stringType (Json.toJson obj);
  };
  external _on : t => string => ('a => unit) => unit = "on" [@@bs.send];
  let on_not_ready_yet socket func => List.map (fun t => _on socket (M.stringify t) (func t)) M.all;
  let on socket t func => {
    /*let assumeObjWithFirstElemTag : 'a => 'b = [%bs.raw {|
      function(obj) {
        if (Object.prototype.toString.call(obj) === "[object Array]") {
          var ret = obj[1];
          if (obj[0] != -1) {
            ret.tag = obj[0];
          }
          return ret;
        }
        return obj;
      }
    |}];*/
    _on socket (M.stringify t) (fun obj => func (Json.fromJson obj));
  };
};
