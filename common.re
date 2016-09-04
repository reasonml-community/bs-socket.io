/*
 * vim: set ft=rust:
 * vim: set ft=reason:
 */
type t =
  | Message
  | MessageOnEnter;

let stringify t =>
  switch t {
  | Message => "message"
  | MessageOnEnter => "otherthing"
  };

let all = [Message, MessageOnEnter];
