let socket = Client.create ();

let chatarea = Web.Document.getElementById "chatarea";

Client.on
  socket
  "message"
  (
    fun x => {
      let innerHTML = Web.Element.getInnerHTML chatarea;
      Web.Element.setInnerHTML
        chatarea
        (innerHTML ^ "<div><span style='color:red'>Other</span>: " ^ Web.toString x ^ "</div>")
    }
  );

let sendbutton = Web.Document.getElementById "sendbutton";

let chatinput = Web.Document.getElementById "chatinput";

Web.Element.addEventListener
  sendbutton "click" (fun _ => Client.emit socket "message" (Web.Element.getValue chatinput));

/*
 /* same example with js_of_ocaml */

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
                "<div><span style='color:red'>Other</span>: " ^
                Js.to_string (meth_call s "toString" [||]) ^
                "</div>"
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
    inject (Js.wrap_callback (fun e => socket#emit "message" (get chatinput "value")))
  |];

  */
