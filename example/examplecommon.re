type dataT =
  | Data string
  | OrOthers;

type data2T = string;

type t 'a =
  | Message :t dataT
  | MessageOnEnter :t data2T
  | UnusedMessageType :t 'a;


/**
 * This is not great. I'd like to not force the user to have to know about
 * locally abstract data types.
 * The reaosn this is needed is because we're calling stringify on different t
 * 'a with different 'a, so  we want the type checker to allow multiple
 * different calls.
 * If we remove the (type a) and used 'a instead, the inference algorithm will
 * pick the first usage of this function to concretize the type 'a and will
 * error on any other call to stringify that receives a 'a of a
 * different type.
 *
 * Say we do:
 *
 *   stringify Message;
 *
 * and later:
 *
 *   stringify MessageOnEnter
 *
 * It will error because MessageOnEnter has type `t data2T` but in the first
 * call, Message had type `t dataT` and data2T != dataT.
 * This (type a) is basically allowing those two calls to coexist.
 */
let stringify (type a) (t: t a) =>
  switch t {
  | Message => "Message"
  | MessageOnEnter => "MessageOnEnter"
  | UnusedMessageType => "UnusedMessageType"
  };
