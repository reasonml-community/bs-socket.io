type serverT;

type socketT;

module Make:
  (M: Common.M_t) =>
  {
    let create: unit => serverT;
    let createWithHttp: 'a => serverT;
    type createOptionsT;
    let makeOptions:
      (
        ~pingTimeout: int=?,
        ~pingInterval: int=?,
        ~maxHttpBufferSize: int=?,
        ~transports: list(string)=?,
        ~allowUpgrades: bool=?,
        ~perMessageDeflate: int=?,
        ~httpCompression: int=?,
        ~cookie: string=?,
        ~cookiePath: string=?,
        ~wsEngine: string=?,
        unit
      ) =>
      createOptionsT;
    let createWithOptions: createOptionsT => serverT;
    let createWithHttpAndOption: ('a, createOptionsT) => serverT;
    let createWithPort: (int, createOptionsT) => serverT;
    let serveClient: (serverT, bool) => serverT;
    let path: (serverT, string) => serverT;
    let adapter: (serverT, 'a) => serverT;
    let origins: (serverT, string) => serverT;
    let originsWithFunc: (serverT, ('a, bool) => unit) => serverT;
    let close: serverT => unit;
    let attach: (serverT, 'a, createOptionsT) => serverT;
    let attachWithPort: (serverT, int, createOptionsT) => serverT;
    let emit: (serverT, M.t('a), 'a) => unit;
    module Socket: {
      let getId: socketT => string;
      let getRooms: socketT => Js.t('a);
      let getHandshake: socketT => Js.t('a);
      let on: (socketT, M.t('a), 'b => unit) => unit;
      let emit: (socketT, M.t('a), 'a) => unit;
      type broadcastT;
      let broadcast: (socketT, M.t('a), 'b) => unit;
      let join: (socketT, string, 'a => unit) => socketT;
      let leave: (socketT, string, 'a => unit) => socketT;
      let to_: (socketT, string) => socketT;
      let compress: (socketT, bool) => socketT;
      let disconnect: (socketT, bool) => socketT;
      let use: (socketT, ('a, unit => unit) => unit) => unit;
      let once: (socketT, M.t('a), 'b => 'c) => unit;
      type volatileT;
      let getVolatile: socketT => volatileT;
      let volatileEmit: (socketT, M.t('a), 'a) => unit;
    };
    let onConnect: (serverT, socketT => unit) => unit;
  };
