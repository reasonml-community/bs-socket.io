/* This is kind of a hack to make the example be as copy-pastable as possible.
   Without this module, the example can't do `BsSocket.Server.Make`, it'd have to access `Server.Make`
   directly, because bsb doesn't expose the namespace to the dev sources :(

                            Ben - May the 4th 2018
*/


module Server = Server;
module Client = Client;
module Namespace = Namespace;

module V2 = V2;
