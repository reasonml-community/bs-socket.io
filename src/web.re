/**
 * vim: set ft=rust:
 * vim: set ft=reason:
 *
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
