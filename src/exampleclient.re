/*
 * vim: set ft=rust:
 * vim: set ft=reason:
 */
module CustomClient = Client.Client Examplecommon;

let socket = CustomClient.create ();

let chatarea = Web.Document.getElementById "chatarea";

CustomClient.on
  socket
  Examplecommon.Message
  (
    fun x =>
      switch x {
      | Examplecommon.Data s =>
        let innerHTML = Web.Element.getInnerHTML chatarea;
        Web.Element.setInnerHTML
          chatarea (innerHTML ^ "<div><span style='color:red'>Message</span>: " ^ s ^ "</div>")
      | Examplecommon.OrOthers => print_endline "OrOthers"
      }
  );

CustomClient.on
  socket
  Examplecommon.MessageOnEnter
  (
    fun x =>
      switch x {
      | Examplecommon.Data s =>
        let innerHTML = Web.Element.getInnerHTML chatarea;
        Web.Element.setInnerHTML
          chatarea
          (innerHTML ^ "<div><span style='color:red'>MessageOnEnter</span>: " ^ s ^ "</div>")
      | Examplecommon.OrOthers => print_endline "OrOthers"
      }
  );

let sendbutton = Web.Document.getElementById "sendbutton";

let chatinput = Web.Document.getElementById "chatinput";

Web.Element.addEventListener
  sendbutton
  "click"
  (
    fun _ =>
      CustomClient.emit
        socket Examplecommon.Message (Examplecommon.Data (Web.Element.getValue chatinput))
  );

Web.Document.addEventListener
  "keyup"
  (
    fun e =>
      if (Web.Event.isEnterKey e) {
        CustomClient.emit
          socket
          Examplecommon.MessageOnEnter
          (Examplecommon.Data (Web.Element.getValue chatinput));
        Web.Element.setValue chatinput ""
      }
  );
