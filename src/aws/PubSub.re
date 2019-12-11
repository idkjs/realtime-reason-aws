type t;
type stream;
[@bs.module "@aws-amplify/pubsub"] external pubsub: t = "default";
[@bs.send] external configurePubSub: (t, AwsExports.t) => unit = "configure";
let configurePubSub = config => configurePubSub(pubsub, config);
// type event = {
//   provider: Js.Json.t,
//   value,
// }
// and value = {data: Types.onCreateMessage};
// type errorValue = {message: string};
// type subscriptionObserver = {
//   closed: bool,
//   next: event => unit,
//   error: errorValue => unit,
//   complete: unit,
// };
type event = {
  provider: Js.Json.t,
  value,
}
and value = {data: Types.onCreateMessage};
type errorValue = {message: string};
type subscriptionObserver = {
  closed: bool,
  next: event => unit,
  error: errorValue => unit,
  complete: unit,
};
type operation = Types.graphqlOperation => Js.Promise.t(Types.executionResult);
[@bs.send] external asyncIterator: (t, string) => stream = "asyncIterator";
[@bs.send.pipe: t]
external subscribe:
  (t, subscriptionObserver) => Observable.t(subscriptionObserver) =
  "subscribe";
let subscribe = subscriptionObserver =>
  subscribe(pubsub, subscriptionObserver);

// let subscription: t => subscriptionObserver =
//   subscribe(pubsub,subscriptionObserver) => {
    // next: event => {
    //   Js.log2(
    //     "Subscription: ",
    //     Utils.jsonStringify(event.value.data, Js.Nullable.null, 2),
    //   );
    //   Js.log2("EVENT: ", Utils.jsonStringify(event, Js.Nullable.null, 2));
    //   // setDisplay(true);
    //   let message = event.value.data.message;
    //   Js.log2("message", message);
    // },
//     closed: false,
//     error: errorValue => Js.log2("errorValue", errorValue),
//     complete: Js.log("complete"),
//   };
// let resolvers: t => subscriptionObserver =
//   pubsub => {
//     "state": {
//       "subscribe": () => asyncIterator(pubsub, "STATE"),
//     },
//   };
// let subscription = graphqlOperation => subscribe(pubsub, graphqlOperation);
// type subscriptionObserver = {
// 	closed: bool,
// 	next: value('a) => unit,
// 	error: errorValue => unit,
// 	complete: (unit)
// };