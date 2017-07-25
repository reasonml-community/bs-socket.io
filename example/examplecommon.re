type dataT =
  | Data string
  | OrOthers;

type data2T = string;

type t 'a =
  | Message: t dataT
  | MessageOnEnter: t data2T
  | UnusedMessageType : t 'a;

let stringify (type a) (t : t a) =>
  switch t {
  | Message => "Message"
  | MessageOnEnter => "MessageOnEnter"
  | UnusedMessageType => "UnusedMessageType"
  };