/* Created a bunch of modules to keep things clean. This is just for demo purposes. */
module Path = {
  type pathT;
  [@bs.module "path"] [@bs.splice]
  external join : array(string) => string = "";
};

module Express = {
  type expressT;
  [@bs.module] external express : unit => expressT = "";
  [@bs.send] external use : (expressT, string) => unit = "use";
  [@bs.module "express"] external static : string => string = "static";
  type responseT;
  [@bs.send] external sendFile : (responseT, string, 'a) => unit = "sendFile";
  [@bs.send]
  external get : (expressT, string, ('a, responseT) => unit) => unit = "get";
};

module Http = {
  type http;
  [@bs.module "http"] external create : Express.expressT => http = "Server";
  [@bs.send] external listen : (http, int, unit => unit) => unit = "";
};

/* ------------ Real API demo stars here ------------ */
let app = Express.express();

let http = Http.create(app);

[@bs.val] external __dirname : string = "";

Express.use(app, Express.static(Path.join([|__dirname, ".."|])));

Express.get(app, "/", (_, res) =>
  Express.sendFile(res, "index.html", {"root": __dirname})
);

module MyServer = BsSocket.Server.Make(ExampleMessages);

let io = MyServer.createWithHttp(http);

MyServer.onConnect(
  io,
  socket => {
    open MyServer;
    print_endline("Got a connection!");
    let socket =
      Socket.join(socket, "someRoom", (~err) => Js.log2("error is:", err));
    /* Polymorphic pipe which actually knows about ExampleCommon.t from InnerServer */
    Socket.on(
      socket,
      data => {
        Socket.broadcast(socket, data);
        Socket.emit(socket, data);
      },
    );
  },
);

Http.listen(http, 3000, () => print_endline("listening on *:3000"));
