/* Module to implement to get an instance of the server module.

     Example:

      module InnerServer = Server.Server ({
        type t 'a = string;
        let stringify t => t;
      });

      let io = InnerServer.create ();

      `stringify` is used to turn the event type to a string which is used to
      identify the packet.
   */
module type M_t = {type t 'a; let stringify: t 'a => string;};
