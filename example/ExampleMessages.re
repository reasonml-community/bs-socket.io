type dataT =
  | Data(string)
  | OrOthers;

type data2T = string;

type shared =
  | Message(dataT)
  | MessageOnEnter(data2T);

type clientToServer =
  | Shared(shared)
  | Hi;

/* In this simple example, the server has no unique messages, unlike
   the client, which is polite and says hi after connecting (in addition
   to sending all the shared message types). */
type serverToClient = shared;
