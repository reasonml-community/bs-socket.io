let socket = V2.Client.createWithUrl("https://localhost:8080");

open V2.Socket;

let logConnectAttempt = attempt => {
  Js.log("Attempt number: " ++ Belt.Int.toString(attempt));
};

// Connect Events
socket->onConnect(() => {Js.log("connected")});
socket->onConnectError(err => {Js.log("Error happened: " ++ err.message)});

// Reconnect Events
socket->onReconnect(logConnectAttempt);
socket->onReconnecting(logConnectAttempt);
socket->onReconnectFailed(() => {Js.log("reconnect failed")});

socket->onEvent("news", arg => {
  // Different ways to handle `arg` here
  // 1) Either use Js.Json.classify (low level JSON decoding layer)
  // 2) Use libraries such as bs-json / bs-decode / ...
  // 3) Use a unsafe raw / %identity function for unsafe coercion

  /*switch(arg->Js.Json.classify) {*/
  /*| Js.Json.String => */
  /*};*/
  let result =
    Js.Json.stringifyAny(arg)
    ->Belt.Option.getWithDefault("Could not stringify arg");
  Js.log("news event happened: " ++ result);
});

socket->onEvent2("news", (arg1, arg2) => {
  Js.log3("News event happened", arg1, arg2)
});

socket->onEvent3("news", (arg1, arg2, arg3) => {
  Js.log4("News event happened", arg1, arg2, arg3)
});

socket->onPing(() => Js.log("ping occurred"));
socket->onPong(latency => Js.log2("pong occurred. Latency: ", latency));

socket->onError(err => {Js.log("An error event occurred: " ++ err.message)});

socket->connect;
