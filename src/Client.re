type t = Socket.socketT;

/* Socket.io docs: https://socket.io/docs/client-api/#io-url-options */
[@bs.new] external create: unit => t = "io";
[@bs.new] external createWithUrl: string => t = "io";
