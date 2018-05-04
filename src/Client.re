module Make = (M: Common.M_t) => {
  type t;
  [@bs.new] external create : unit => t = "io";
  [@bs.send] external _emit : (t, string, 'a) => unit = "emit";
  let emit = (socket, obj: M.t) =>
    _emit(socket, "message", Json.toValidJson(obj));
  [@bs.send] external _on : (t, string, 'a => unit) => unit = "on";
  let on = (socket, func: M.t => unit) =>
    _on(socket, "message", obj => func(Json.fromValidJson(obj)));
};
