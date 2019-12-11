open Types;

let resolvers: PubSub.t => subscription =
  pubsub => {
    "state": {
      "subscribe": () => PubSub.asyncIterator(pubsub, "STATE"),
    },
  };
// let resolvers: PubSub.t => subscription =
//   pubsub => {
//     state->subscribe() => PubSub.asyncIterator(pubsub, "STATE"),
//     },
//   };
  // let resolvers: PubSub.t => subscription =
  //   pubsub => {
  //     state: {
  //       subscribe: () => PubSub.asyncIterator(pubsub, "STATE"),
  //     },
  //   };