type t;

external getName : t => string = "name" [@@bs.get];

external clients : t => ('a => list string => unit [@bs]) => unit = "clients" [@@bs.send];

external use : t => (Server.socketT => (unit => unit) => unit [@bs]) => unit = "use" [@@bs.send];

external default : Server.serverT => t = "sockets" [@@bs.send];

external from : Server.serverT => string => t = "of" [@@bs.send];
