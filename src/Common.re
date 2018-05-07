/* CR mrussell: fix comment */
/* Module to implement to get an instance of the server module.

               Example:

                module InnerServer = Server.Make({
                  type t('a) = string;
                });

                let io = InnerServer.create();

                `stringify` is used to turn the event type to a string which is used to
                identify the packet.
   */
/* CR mrussell: rename module Messages */
module type S = {type clientToServer; type serverToClient;};
