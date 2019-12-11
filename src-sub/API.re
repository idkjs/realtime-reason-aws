type t;
[@bs.module "@aws-amplify/api"] external api: t = "default";
[@bs.send] external configure: (t, AwsExports.t) => unit = "configure";
let configure = config => configure(api, config);

[@bs.send]
external _graphql:
  (t, Types.graphqlOperation) => Js.Promise.t(Types.executionResult) =
  "graphql";
let mutate: Types.operation =
  graphqlOperation => {
    _graphql(api, graphqlOperation);
  };

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
type event = Js.Json.t;
type pubsub;
[@bs.module "@aws-amplify/pubsub"] external pubsub: pubsub = "default";
[@bs.send.pipe: pubsub]
external subscribe: (pubsub, Types.graphqlOperation) => Observable.t(event) =
  "subscribe";
// type subscriptionObserver = {
// 	closed: bool,
// 	next: value('a) => unit,
// 	error: errorValue => unit,
// 	complete: (unit)
// };
// type event = {
//   provider: Js.Json.t,
//   value,
// }
// and value = {data: Types.onCreateMessage};

// type payload = {next: event => unit};
type nextLink = Types.operation => Observable.t(Types.executionResult);
// let subscription = graphqlOperation => subscription(api, graphqlOperation);

// let subscription = graphqlOperation => {
//   subscribe(pubsub, graphqlOperation)
//   |> Observable.make((observer: SubscriptionObserver.t(event)) => {
//        observer->(SubscriptionObserver.next(nextLink));
//        ignore;
//      });
// };