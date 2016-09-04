/*
 * vim: set ft=rust:
 * vim: set ft=reason:
 */
let module CustomClient = Client.Client Common;

let socket = CustomClient.create ();

let chatarea = Web.Document.getElementById "chatarea";

/* CustomClient.on
   socket
   Common.Message
   (
     fun x => {
       let innerHTML = Web.Element.getInnerHTML chatarea;
       Web.Element.setInnerHTML
         chatarea
         (innerHTML ^ "<div><span style='color:red'>Message</span>: " ^ Web.toString x ^ "</div>")
     }
   ); */
CustomClient.on
  socket
  (
    fun t x =>
      switch t {
      | Common.Message =>
        let innerHTML = Web.Element.getInnerHTML chatarea;
        Web.Element.setInnerHTML
          chatarea
          (innerHTML ^ "<div><span style='color:red'>Message</span>: " ^ Web.toString x ^ "</div>")
      | Common.MessageOnEnter =>
        let innerHTML = Web.Element.getInnerHTML chatarea;
        Web.Element.setInnerHTML
          chatarea
          (
            innerHTML ^
            "<div><span style='color:red'>MessageOnEnter</span>: " ^ Web.toString x ^ "</div>"
          )
      }
  );

let sendbutton = Web.Document.getElementById "sendbutton";

let chatinput = Web.Document.getElementById "chatinput";

Web.Element.addEventListener
  sendbutton
  "click"
  (fun _ => CustomClient.emit socket Common.Message (Web.Element.getValue chatinput));

Web.Document.addEventListener
  "keyup"
  (
    fun e =>
      if (Web.Event.isEnterKey e) {
        CustomClient.emit socket Common.MessageOnEnter (Web.Element.getValue chatinput);
        Web.Element.setValue chatinput ""
      }
  );
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
