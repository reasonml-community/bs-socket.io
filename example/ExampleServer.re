/* Created a bunch of modules to keep things clean. This is just for demo purposes. */
module Path = {
  type pathT;
  [@bs.module "path"] [@bs.splice] external join: array(string) => string = "join";
};

module Express = {
  type expressT;
  [@bs.module] external express: unit => expressT = "express";
  [@bs.send] external use: (expressT, string) => unit = "use";
  [@bs.module "express"] external static: string => string = "static";
  type responseT;
  [@bs.send] external sendFile: (responseT, string, 'a) => unit = "sendFile";
  [@bs.send] external get: (expressT, string, ('a, responseT) => unit) => unit = "get";
};

module Http = {
  type http;
  [@bs.module "http"] external create: Express.expressT => http = "Server";
  [@bs.send] external listen: (http, int, unit => unit) => unit = "listen";
};

/* ------------ Real API demo stars here ------------ */
let app = Express.express();

let http = Http.create(app);

let __dirname =
  switch ([%external __dirname]) {
  | None => failwith("Should not happen")
  | Some(d) => d
  };
Express.use(app, Express.static(Path.join([|__dirname, ".."|])));

Express.get(app, "/", (_, res) => Express.sendFile(res, "index.html", {"root": __dirname}));

// module MyServer = BsSocket.Server.Make(ExampleMessages);

let io = Server.createWithHttp(http);

io->Server.onConnect(socket => {
  print_endline("Got a connection!");
  let socket = socket->Socket.join("someRoom");
  /* Polymorphic pipe which actually knows about ExampleCommon.t from InnerServer */
  socket->Socket.on("login", (. data) =>
    switch (data) {
    | SharedTypes.Hi =>
      Js.log("oh, hi client.");
      Js.log("Sorry I can't say hi back.  Try uncommenting the line below to see why.");
    /* Socket.emit(socket, Hi); */
    }
  );
  socket->Socket.on("message", (. data) => {
    Server.emitBroadcast(socket, "myEvent", data);
    Socket.emit(socket, "myEvent", data);
  });
});

Http.listen(http, 3000, () => print_endline("listening on *:3000"));
