module CustomClient = Client.Client Examplecommon;

open ReasonJs;

/* Patches for ReasonJs */
external addKeyUpListener : _ [@bs.as "keyup"] => (Dom.KeyboardEvent.t => unit) => unit =
  "addEventListener" [@@bs.send.pipe : Dom.Document.t];

external setValue : Dom.HtmlElement.t_htmlElement => string => unit = "value" [@@bs.set];
/******/

let socket = CustomClient.create ();

let getElementOrFail (name: string) =>
  switch (Dom.Document.getElementById name Dom.document) {
  | None => failwith ("Couldn't find element [" ^ name ^ "]")
  | Some el => el
  };

let chatarea = getElementOrFail "chatarea";

let sendbutton = getElementOrFail "sendbutton";

let chatinput = getElementOrFail "chatinput";

let htmlchatinput =
  switch (Dom.Element.asHtmlElement chatinput) {
  | None => failwith "error"
  | Some e => e
  };

CustomClient.on
  socket
  Examplecommon.Message
  (
    fun x =>
      switch x {
      | Examplecommon.Data s =>
        let innerHTML = Dom.Element.innerHTML chatarea;
        Dom.Element.setInnerHTML
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
        let innerHTML = Dom.Element.innerHTML chatarea;
        Dom.Element.setInnerHTML
          chatarea
          (innerHTML ^ "<div><span style='color:red'>MessageOnEnter</span>: " ^ s ^ "</div>")
      | Examplecommon.OrOthers => print_endline "OrOthers"
      }
  );

Dom.Element.addEventListener
  "click"
  (
    fun _ =>
      CustomClient.emit
        socket Examplecommon.Message (Examplecommon.Data (Dom.HtmlElement.value htmlchatinput))
  )
  sendbutton;

addKeyUpListener
  (
    fun e =>
      if (Dom.KeyboardEvent.key e == "Enter") {
        CustomClient.emit
          socket
          Examplecommon.MessageOnEnter
          (Examplecommon.Data (Dom.HtmlElement.value htmlchatinput));
        setValue htmlchatinput ""
      }
  )
  Dom.document;
