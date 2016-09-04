/*
 * vim: set ft=rust:
 * vim: set ft=reason:
 */
/* I did this instead of
 *
 *   external __dirname : string = "" [@@bs.val];
 *
 * because the latter will give a merlin error. It doesn't seem to be valid in ocaml to bind to just
 * values instead of functions.
 */
let __dirname: string = [%bs.raw "__dirname"];

/* Created a bunch of modules to keep things clean. This is just for demo purposes. */
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
  type http;
  external create : Express.expressT => http = "Server" [@@bs.module "http"];
  external listen : http => int => (unit => unit) => unit = "" [@@bs.send];
};

/* ------------ Real API demo stars here ------------ */
let app = Express.express ();

let http = Http.create app;

Express.use app (Express.static (Path.join __dirname));

Express.get app "/" (fun req res => Express.sendFile res "index.html" [%bs.obj {root: __dirname}]);

let module InnerServer = Server.Server Common;

let io = InnerServer.createWithHttp http;

InnerServer.close io;

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
        Socket.emit socket Common.UnusedMessageType data
      };
      /* Polymorphic pipe which actually knows about Common.t from InnerServer */
      Socket.on socket pipe
    }
  );

Http.listen http 3000 (fun () => print_endline "listening on *:3000");
