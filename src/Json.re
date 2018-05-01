/* @Performance
     Special serialization logic because we cannot serialize variants using Json.stringify.
     The reason for that is that Variants are represented as arrays under the hood with a property .tag
     representing the tag kind. That property isn't one that's normally on array so Json.stringify won't serialize it.
     We have to resort to using our own encoding.
           Ben - July 24 2017
   */
let toValidJson = [%raw
  (o) =>
  {|
  switch (typeof o){
    case "boolean":
    case "number":
    case "string":
      return o;
    case "function":
      throw new Error("Cannot serialize functions");
    case "object":
      if (Array.isArray(o)){
        return [o.hasOwnProperty("tag") ? o.tag : -1, o.map(toValidJson)];
      }
      throw new Error("Cannot serialize unidentified object [" + o + "].")
  }
|}
];

let fromValidJson = [%raw
  (o) =>
  {|
  switch (typeof o){
    case "boolean":
    case "number":
    case "string":
      return o;
    case "function":
      throw new Error("Cannot deserialize functions");
    case "object":
      if (Array.isArray(o)){
        var first = o[0]
        if (first == -1){
          return o[1].map(fromValidJson);
        } else {
          var a = o[1].map(fromValidJson);
          a.tag = first;
          return a
        }
      }
      throw new Error("Cannot deserialize unidentified object [" + o + "].")
  }
|}
];
