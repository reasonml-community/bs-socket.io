module Make = (M: Common.M_t) => {
  type t;
  [@bs.new] external create : unit => t = "io";
  [@bs.send] external _emit : (t, string, 'a) => unit = "emit";
  let emit = (socket, message: M.t('a), obj: 'a) =>
    _emit(socket, M.stringify(message), Json.toValidJson(obj));
  [@bs.send] external _on : (t, string, 'a => unit) => unit = "on";
  let on = (socket, message: M.t('a), func: 'a => unit) =>
    _on(socket, M.stringify(message), (obj) => func(Json.fromValidJson(obj)));
};
