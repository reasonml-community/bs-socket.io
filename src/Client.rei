module Make:
  (M: BsSocket.Common.M_t) =>
  {
    type t;
    let create: unit => t;
    let _emit: (t, string, 'a) => unit;
    let emit: (t, M.t('a), 'a) => unit;
    let _on: (t, string, 'a => unit) => unit;
    let on: (t, M.t('a), 'a => unit) => unit;
  };
