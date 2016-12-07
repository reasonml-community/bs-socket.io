/*
 * vim: set ft=rust:
 * vim: set ft=reason:
 */
let module CustomClient = Client.Client Common;

let socket = CustomClient.create ();

let chatarea = Web.Document.getElementById "chatarea";

/* API is great because it forces you to exhaustively match on all possible kinds of messages */
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
      | typ => print_endline @@ "Event '" ^ Common.stringify typ ^ "' not implemented yet."
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
