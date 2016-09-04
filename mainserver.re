/*
 * vim: set ft=rust:
 * vim: set ft=reason:
 */
external __dirname : string = "" [@@bs.val];

let module Path = {
  external join : string => string = "join" [@@bs.module "path"];
};

let module Express = {
  type expressT;
  external express : unit => expressT = "" [@@bs.module];
  external use : expressT => string => unit = "use" [@@bs.send];
  external static : string => string = "static" [@@bs.module "express"];
  type responseT;
  external sendFile : responseT => string => 'a => unit = "sendFile" [@@bs.send];
  external get : expressT => string => ('a => responseT => unit [@bs]) => unit = "get" [@@bs.send];
};

let module Http = {
  external create : Express.expressT => Server.http = "Server" [@@bs.module "http"];
  external listen : Server.http => int => (unit => unit) => unit = "" [@@bs.send];
};

let app = Express.express ();

let http = Http.create app;

Express.use app (Express.static __dirname);

Express.get app "/" (fun req res => Express.sendFile res "index.html" [%bs.obj {root: __dirname}]);

let io = Server.create http;

Server.onConnect
  io
  (
    fun socket => {
      print_endline "Got a connection!";
      Server.on
        socket
        "message"
        (
          fun data => {
            Server.broadcast socket "message" data;
            Server.emit socket "message" data
          }
        )
    }
  );

Http.listen http 3000 (fun () => print_endline "listening on *:3000");
