type dataT =
  | Data(string)
  | OrOthers;

type data2T = string;

type t =
  | Message(dataT)
  | MessageOnEnter(data2T)
  | UnusedMessageType;
