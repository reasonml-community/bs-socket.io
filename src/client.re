type t;

external create : unit => t = "io" [@@bs.new];

external emit : t => string => Js.t 'a => unit = "emit" [@@bs.send];

external on : t => string => (Js.t 'a => unit [@bs]) => unit = "on" [@@bs.send];
