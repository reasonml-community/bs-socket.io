module Make:
  (M: Common.M_t) =>
  {
    type t;
    let create: unit => t;
    let emit: (t, M.t('a), 'a) => unit;
    let on: (t, M.t('a), 'a => unit) => unit;
  };
