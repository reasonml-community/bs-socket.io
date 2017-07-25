/**
 * All credit goes to Cheng Lou. It was just too hard to figure out jengaboot + bucklescript for now.
 * Copy pasted from https://github.com/chenglou/reason-js
 **/
external toString : Js.t 'a => string = "toString" [@@bs.send];

let module Event = {
  type eventT;
  let isEnterKey: eventT => bool = [%bs.raw
    {|
    function (e) {
      return e.which === 13;
    }
  |}
  ];
};

/* Created a bunch of modules to keep things clean. This is just for demo purposes. */
let module Element = {
  type elementT;
  external setInnerHTML : elementT => string => unit = "innerHTML" [@@bs.set];
  external getInnerHTML : elementT => string = "innerHTML" [@@bs.get];
  external setValue : elementT => string => unit = "value" [@@bs.set];
  external getValue : elementT => string = "value" [@@bs.get];
  external addEventListener : elementT => string => (Event.eventT => unit) => unit = "addEventListener" [@@bs.send];
};

let module Document = {
  external getElementById : string => Element.elementT = "document.getElementById" [@@bs.val];
  external addEventListener : string => (Event.eventT => unit) => unit = "document.addEventListener" [@@bs.val];
};

let module Window = {
  type intervalIdT;
  external setInterval : (unit => unit) => int => intervalIdT = "window.setInterval" [@@bs.val];
  external clearInterval : intervalIdT => unit = "window.clearInterval" [@@bs.val];
};

let module Console = {
  external log : 'anything => unit = "console.log" [@@bs.val];
};


module CustomClient = Client.Client Examplecommon;

let socket = CustomClient.create ();

let chatarea = Document.getElementById "chatarea";

CustomClient.on
  socket
  Examplecommon.Message
  (
    fun x =>
      switch x {
      | Examplecommon.Data s =>
        let innerHTML = Element.getInnerHTML chatarea;
        Element.setInnerHTML
          chatarea (innerHTML ^ "<div><span style='color:red'>Message</span>: " ^ s ^ "</div>")
      | Examplecommon.OrOthers => print_endline "OrOthers"
      }
  );

CustomClient.on
  socket
  Examplecommon.MessageOnEnter
  (
    fun s => {
      let innerHTML = Element.getInnerHTML chatarea;
      Element.setInnerHTML
        chatarea
        (innerHTML ^ "<div><span style='color:red'>MessageOnEnter</span>: " ^ s ^ "</div>")
  });

let sendbutton = Document.getElementById "sendbutton";

let chatinput = Document.getElementById "chatinput";

Element.addEventListener
  sendbutton
  "click"
  (
    fun _ =>
      CustomClient.emit
        socket Examplecommon.Message (Examplecommon.Data (Element.getValue chatinput))
  );

Document.addEventListener
  "keyup"
  (
    fun e =>
      if (Event.isEnterKey e) {
        CustomClient.emit
          socket
          Examplecommon.MessageOnEnter
          (Element.getValue chatinput);
        Element.setValue chatinput ""
      }
  );
