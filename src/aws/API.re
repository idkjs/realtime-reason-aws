type t;
[@bs.module "@aws-amplify/api"] external api: t = "default";
[@bs.send] external configure: (t, AwsExports.t) => unit = "configure";
let configure = config => configure(api, config);

[@bs.send]
external _graphql:
  (t, Types.graphqlOperation) => Js.Promise.t(Types.executionResult) =
  "graphql";
type pubsub;
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
type objectWithCallback = {
  // y: option({. z: option(unit => int)}),
  // x: option(unit => unit),
  // w: option(unit => int),
  closed: option(bool),
  next: option(event => unit),
  error: option(errorValue => unit),
  complete: option(unit=>unit),
};
let objectWithCallback: objectWithCallback = {
  next: Some(event => Js.log(event)),
  error: Some(errorValue => Js.log(errorValue)),
  closed: Some(true),
  complete: Some(_=>Js.log("complete"))
};

[@bs.module "@aws-amplify/pubsub"] external pubsub: t = "default";
[@bs.send]
external _subscribe:
  (t, objectWithCallback) => Observable.t(objectWithCallback) =
  "subscribe";
[@bs.send]
external _graphqlSub:
  (t, Types.graphqlOperation) => Observable.t(objectWithCallback) =
  "graphql";
[@bs.send]
external _graphqlSubWonka:
  (t, Types.graphqlOperation) => Wonka.observableT('a) =
  "graphql";
let mutate: Types.operation =
  graphqlOperation => {
    _graphql(api, graphqlOperation);
  };
let sub =
  graphqlOperation => {
    _graphqlSub(api, graphqlOperation);
  };
let subWithWonka =
  graphqlOperation => {
    _graphqlSubWonka(api, graphqlOperation);
  };
type subOperation = Types.operation => Observable.t(subscriptionObserver);
type subscription = Types.operation => subOperation;
// let subscription: subscription =
//   (graphqlOperation: Types.graphqlOperation, subscriptionObserver) => {
//     mutate(graphqlOperation)
//     |> Js.Promise.resolve(res)
//     |> _subscribe(
//          pubsub,
//          subscriptionObserver /*   Observable.make((observer: SubscriptionObserver.t(event)) => */ /*   _graphql(api, graphqlOperation)*/,
//        ) /* let subscription = graphqlOperation => */ /* }*/ /* 	complete: (unit*/;
//   } /* 	error: errorValue => unit*/;

// type subscriptionOptions = {
// };
// [@bs.send.pipe: t]
// external subscribe: Types.graphqlOperation => Js.Promise.t(Types.executionResult) = "subscribe";

// Observable.make((observer: SubscriptionObserver.t(int)) => {
//   observer->(SubscriptionObserver.next(10));
//   ignore;
// })
// ->Observable.(subscribe(x => Js.log(x)))
// ->ignore;
// open Observable;
// type pubsub;
// [@bs.module "@aws-amplify/pubsub"] external pubsub: pubsub = "default";
// [@bs.send]
// external configurePubSub: (pubsub, AwsExports.t) => unit = "configure";
// let configurePubSub = config => configurePubSub(pubsub, config);
// [@bs.send.pipe: pubsub]
// external subscribe:
//   (pubsub, Types.graphqlOperation) => Observable.t(Types.executionResult) =
//   "subscribe";
// type subscriptionObserver = {
// 	closed: bool,
// 	next: value('a) => unit,