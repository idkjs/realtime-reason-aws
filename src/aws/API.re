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
type pubsub;
[@bs.module "@aws-amplify/pubsub"] external pubsub: pubsub = "default";
[@bs.send.pipe: pubsub]
external subscribe:
  Types.graphqlOperation => Observable.t(Types.executionResult) =
  "subscribe";
// type subscriptionOptions = {topics: string};
// type subscriptionObserver('a) = {
// 	closed: bool,
// 	next: value('a) => unit,
// 	error(errorValue: any): void;
// 	complete(): void;
// };
type event = {
  provider: Js.Json.t,
  value,
}
and value = {data: Types.onCreateMessage};

type payload = {next: event => unit};
type nextLink = Types.operation => Observable.t(Types.executionResult);
// let subscription = graphqlOperation => subscription(api, graphqlOperation);

let subscription = graphqlOperation => {
  _graphql(api, graphqlOperation)
      Observable.make((observer: SubscriptionObserver.t(event)) => {
        observer->(SubscriptionObserver.next(Js.Json.t));
        ignore;
      });
let init = (pubsub: PubSub.t, port: Port.t) => {
// let subscription = graphqlOperation => {
//   _graphql(api, graphqlOperation)
//   |> Observable.make(observer: SubscriptionObserver.t(event))
//      ->Observable.(subscribe(x => Js.log(x)))//  next: event => Js.log2("event", event),
                                              //  event
                                              //  |> Js.log2(
                                              //       "Subscription: ",
                                              //       Utils.jsonStringify(event.value.data, Js.Nullable.null, 2),
                                              //     );
                                              //  Js.log("EVENT: " ++ Utils.jsonStringify(event, Js.Nullable.null, 2));
                                              //  let message = event.value.data.onCreateMessage.message;
                                              //  Js.log2("message", message);
                                              //  ignore;
                                              //  }
                                              //  )
                                              ; //  {
                                              //  ->Observable.(subscribe(x => Js.log(x)));
// };