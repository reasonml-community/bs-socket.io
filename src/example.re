open Js.Unsafe;

let socket = new Socket.t;

let chatarea = meth_call (variable "document") "getElementById" [|inject (Js.string "chatarea")|];

socket#on
  "message"
  (
    fun s =>
      set
        chatarea
        "innerHTML"
        (
          inject (
            Js.string (
              Js.to_string (get chatarea "innerHTML") ^
                "<div><span style='color:red'>Other</span>: " ^ s ^ "</div>"
            )
          )
        )
  );

let button = meth_call (variable "document") "getElementById" [|inject (Js.string "sendbutton")|];

let chatinput = meth_call (variable "document") "getElementById" [|inject (Js.string "chatinput")|];

meth_call
  button
  "addEventListener"
  [|
    inject (Js.string "click"),
    inject (
      Js.wrap_callback (
        fun e => {
          ignore @@ meth_call (variable "console") "log" [|inject (get e "target")|];
          socket#emit "message" (Js.to_string (get chatinput "value"))
        }
      )
    )
  |];
