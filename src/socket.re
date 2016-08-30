open Js.Unsafe;

class t = {
  val _innerSelf = meth_call (variable "window") "io" [||];
  method emit eventName (data: any) :unit =>
    meth_call _innerSelf "emit" [|inject (Js.string eventName), inject data|];
  method on eventName (cb: any => unit) :unit =>
    meth_call
      _innerSelf
      "on"
      [|inject (Js.string eventName), inject (Js.wrap_callback cb)|];
};
