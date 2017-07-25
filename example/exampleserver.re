/* Created a bunch of modules to keep things clean. This is just for demo purposes. */
module Path = {
  type pathT;
  external join : array string => string =
    "" [@@bs.module "path"] [@@bs.splice];
};

module Express = {
  type expressT;
  external express : unit => expressT = "" [@@bs.module];
  external use : expressT => string => unit = "use" [@@bs.send];
  external static : string => string = "static" [@@bs.module "express"];
  type responseT;
  external sendFile : responseT => string => 'a => unit =
    "sendFile" [@@bs.send];
  external get : expressT => string => ('a => responseT => unit) => unit =
    "get" [@@bs.send];
};

module Http = {
  type http;
  external create : Express.expressT => http = "Server" [@@bs.module "http"];
  external listen : http => int => (unit => unit) => unit = "" [@@bs.send];
};

/* ------------ Real API demo stars here ------------ */
let app = Express.express ();

let http = Http.create app;

external __dirname : string = "" [@@bs.val];

Express.use app (Express.static (Path.join [|__dirname, "..", "..", ".."|]));

Express.get
  app "/" (fun _ res => Express.sendFile res "index.html" {"root": __dirname});

module InnerServer = Server.Server Examplecommon;

let io = InnerServer.createWithHttp http;

InnerServer.onConnect
  io
  (
    fun socket => {
      open InnerServer;
      print_endline "Got a connection!";
      let socket = Socket.join socket "someRoom" (fun e => print_endline e);
      let pipe typ data => {
        Socket.broadcast socket typ data;
        Socket.emit socket typ data;
        Socket.emit socket Examplecommon.UnusedMessageType data
      };
      /* Polymorphic pipe which actually knows about Examplecommon.t from InnerServer */
      Socket.on socket Examplecommon.Message (pipe Examplecommon.Message);
      Socket.on
        socket Examplecommon.MessageOnEnter (pipe Examplecommon.MessageOnEnter)
    }
  );

Http.listen http 3000 (fun () => print_endline "listening on *:3000");
