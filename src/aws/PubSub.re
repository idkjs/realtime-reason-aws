// module PubSub = {
//   type t;
//   type stream;
//   [@bs.module "@aws-amplify/pubsub"] external pubsub: t = "default";
//   [@bs.send]
//   external publishState: (t, [@bs.as "STATE"] _, 'event) => unit = "publish";

//   [@bs.send] external asyncIterator: (t, string) => stream = "asyncIterator";
// };
// open PubSub;
// // type t;
// // [@bs.module "@aws-amplify/pubsub"] external pubsub: t = "default";
// type asyncIterable;
// [@bs.send] external asyncIterator: PubSub.t =>  asyncIterable = "asyncIterator";
// [@bs.send] external subscribe: (PubSub.t, 'payload) => unit = "subscribe";

// type stream;

// [@bs.send]
// external publish: (PubSub.t, [@bs.as "STATE"] _, 'a) => unit = "publish";

// // [@bs.send]
// // external asyncIterator: (PubSub.t, string) => stream = "asyncIterator";

// type subscription = {. "state": {. "subscribe": unit => PubSub.stream}};
// let subscription: PubSub.t => subscription =
//   pubsub => {
//     "state": {
//       "subscribe": () => asyncIterator(PubSub.pubsub, "STATE"),
//     },
//   };

type t;
type stream;

[@bs.send]
external publishState: (t, [@bs.as "STATE"] _, 'a) => unit =
  "publish";

[@bs.send] external asyncIterator: (t, string) => stream = "asyncIterator";