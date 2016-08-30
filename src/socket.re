open Js.Unsafe;

class t = {
  val _innerSelf = meth_call (variable "window") "io" [||];
  method emit eventName (data: string) :unit =>
    meth_call _innerSelf "emit" [|inject (Js.string eventName), inject (Js.string data)|];
  method on eventName (cb: string => unit) :unit =>
    meth_call
      _innerSelf
      "on"
      [|inject (Js.string eventName), inject (Js.wrap_callback (fun s => cb (Js.to_string s)))|];
};
