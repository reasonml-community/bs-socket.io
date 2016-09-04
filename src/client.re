/*
 * vim: set ft=rust:
 * vim: set ft=reason:
 */

type t;

external create : unit => t = "io" [@@bs.new];

external emit : t => string => 'a => unit = "emit" [@@bs.send];

external on : t => string => ('a => unit [@bs]) => unit = "on" [@@bs.send];
