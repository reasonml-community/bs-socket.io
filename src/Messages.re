/* The user of this library is intended to define a Messages module of
   type S, which defines the type of the messages that the client can
   send to the server, and the type of the messages that the server can
   send to the client.

   For example, Messages could look something like:

                      module Messages = {
                        type username = string;

                        type clientToServer =
                        | LoginRequest(username);

                        type loginResponse = {
                          username,
                          successful: bool,
                        };

                        type serverToClient =
                        | LoginResponse(loginResponse)
                      };

   It's fairly common for the types of the messages to be the same,
   regardless of whether they're from server to client or vice versa.
   If that's the case, you can do something like:

                      module Messages = {
                        type t = | ... | ...;
                        type clientToServer = t;
                        type serverToClient = t;
                      };


   The Messages module is the what you pass into the Server, Client,
   and Namespace functors to instantiate those modules with your
   particular message types in mind.  For example:

                      module MyServer = Server.Make(Messages)
                      let io = MyServer.create();
   */
module type S = {type clientToServer; type serverToClient;};
