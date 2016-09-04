/*
 * vim: set ft=rust:
 * vim: set ft=reason:
 */
type t =
  | Message
  | MessageOnEnter
  | UnusedMessageType;

let stringify t =>
  switch t {
  | Message => "Message"
  | MessageOnEnter => "MessageOnEnter"
  | UnusedMessageType => "UnusedMessageType"
  };

let all = [Message, MessageOnEnter, UnusedMessageType];
