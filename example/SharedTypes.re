type dataT =
  | Data(string)
  | OrOthers;

type data2T = string;

// Data sent with event named "message".
type shared =
  | Message(dataT)
  | MessageOnEnter(data2T);

// Data sent with event named "login".
type loginT =
  | Hi;
