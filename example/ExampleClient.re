/***
 * All credit goes to Cheng Lou. It was just too hard to figure out jengaboot + bucklescript for now.
 * Copy pasted from https://github.com/chenglou/reason-js
 **/
[@bs.send] external toString : Js.t('a) => string = "toString";

module Event = {
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
module Element = {
  type elementT;
  [@bs.set] external setInnerHTML : (elementT, string) => unit = "innerHTML";
  [@bs.get] external getInnerHTML : elementT => string = "innerHTML";
  [@bs.set] external setValue : (elementT, string) => unit = "value";
  [@bs.get] external getValue : elementT => string = "value";
  [@bs.send]
  external addEventListener : (elementT, string, Event.eventT => unit) => unit =
    "addEventListener";
};

module Document = {
  [@bs.val]
  external getElementById : string => Element.elementT =
    "document.getElementById";
  [@bs.val]
  external addEventListener : (string, Event.eventT => unit) => unit =
    "document.addEventListener";
};

module Window = {
  type intervalIdT;
  [@bs.val]
  external setInterval : (unit => unit, int) => intervalIdT =
    "window.setInterval";
  [@bs.val]
  external clearInterval : intervalIdT => unit = "window.clearInterval";
};

module Console = {
  [@bs.val] external log : 'anything => unit = "console.log";
};

module MyClient = BsSocket.Client.Make(ExampleCommon);

let socket = MyClient.create();

let chatarea = Document.getElementById("chatarea");

MyClient.on(socket, x =>
  switch (x) {
  | ExampleCommon.Message(Data(s)) =>
    let innerHTML = Element.getInnerHTML(chatarea);
    Element.setInnerHTML(
      chatarea,
      innerHTML
      ++ "<div><span style='color:red'>Message</span>: "
      ++ s
      ++ "</div>",
    );
  | ExampleCommon.Message(OrOthers) => print_endline("OrOthers")
  | ExampleCommon.MessageOnEnter(s) =>
    let innerHTML = Element.getInnerHTML(chatarea);
    Element.setInnerHTML(
      chatarea,
      innerHTML
      ++ "<div><span style='color:red'>MessageOnEnter</span>: "
      ++ s
      ++ "</div>",
    );
    | ExampleCommon.UnusedMessageType => assert(false)
  }
);

/*MyClient.on(
    socket,
    ExampleCommon.MessageOnEnter,
    (s) => {
      let innerHTML = Element.getInnerHTML(chatarea);
      Element.setInnerHTML(
        chatarea,
        innerHTML ++ "<div><span style='color:red'>MessageOnEnter</span>: " ++ s ++ "</div>"
      )
    }
  );*/
let sendbutton = Document.getElementById("sendbutton");

let chatinput = Document.getElementById("chatinput");

Element.addEventListener(sendbutton, "click", (_) =>
  MyClient.emit(
    socket,
    ExampleCommon.Message(ExampleCommon.Data(Element.getValue(chatinput))),
  )
);

Document.addEventListener("keyup", e =>
  if (Event.isEnterKey(e)) {
    MyClient.emit(
      socket,
      ExampleCommon.MessageOnEnter(Element.getValue(chatinput)),
    );
    Element.setValue(chatinput, "");
  }
);
