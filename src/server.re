/*
 * vim: set ft=rust:
 * vim: set ft=reason:
 */
type http;

type io;

external create : http => io = "socket.io" [@@bs.module];

type socketT;

external _onConnect : io => string => (socketT => unit [@bs]) => unit = "on" [@@bs.send];

let onConnect io cb => _onConnect io "connection" cb;

external on : socketT => string => ('a => unit [@bs]) => unit = "on" [@@bs.send];

external emit : socketT => string => 'a => unit = "emit" [@@bs.send];

type broadcastT;

external _broadcast : socketT => broadcastT = "broadcast" [@@bs.get];

external _emit : broadcastT => string => 'a => unit = "emit" [@@bs.send];

let broadcast socket str data => _emit (_broadcast socket) str data;
