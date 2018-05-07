type dataT =
  | Data(string)
  | OrOthers;

type data2T = string;

type message =
  | Message(dataT)
  | MessageOnEnter(data2T);

/* CR mrussell: that shouldn't be true */
/* In this simple example, the type of the message that the client
   sends the server and the server sends the client are the same.  */
type clientToServer = message;

type serverToClient = message;
