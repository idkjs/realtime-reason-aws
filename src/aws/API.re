type t;
[@bs.module "@aws-amplify/api"] external api: t = "default";
[@bs.send] external configureApi: (t, AwsExports.t) => unit = "configure";
// let configure = config => configure(api, config);
type pubsub;
[@bs.module "@aws-amplify/pubsub/lib-esm/index"]
external pubsub: pubsub = "default";
[@bs.send]
external configurePubSub: (pubsub, AwsExports.t) => unit = "configure";
let configure = config => {
  configureApi(api, config);
  configurePubSub(pubsub, config);
};
/* this is unused because I haven't figured out how to use it yet, but a cleaner version of this code will use it. */
let listener: Types.observerLike('event) = {
  next: event => {
    Js.log2(
      "Subscription: ",
      Utils.jsonStringify(event.value.data, Js.Nullable.null, 2),
    );
    Js.log2("EVENT: ", Utils.jsonStringify(event, Js.Nullable.null, 2));
    let message = event.value.data.message;
    Js.log2("MESSAGE: ", Utils.jsonStringify(message, Js.Nullable.null, 2));
  },
  error: errorValue => Js.log(errorValue),
  complete: _ => Js.log("COMPLETE"),
};

/* don't have to bind to this? it happens on the aws-amplify side i think.
   The `API.graphql` function returns a `promise` for queries and mutations and an `Observable` for subscriptions. See: https://github.com/aws-amplify/amplify-js/blob/master/packages/api/src/API.ts#L350

   [@bs.module "@aws-amplify/pubsub"] external pubsub: t = "default"; */

[@bs.send]
external _graphql:
  (t, Types.graphqlOperation) => Js.Promise.t(Types.executionResult) =
  "graphql";

let mutate: Types.operation =
  graphqlOperation => {
    _graphql(api, graphqlOperation);
  };
/* our api subscription query on the amplify/api side which returns and observable
   see:https://github.com/aws-amplify/amplify-js/blob/867412030de57fd74078b609252de6f7f81ad331/packages/pubsub/src/PubSub.ts#L149
    */
[@bs.send]
external _subscribe:
  (t, Types.graphqlOperation) =>
  Wonka.observableT(Types.observableLike(Types.observerLike('value))) =
  "graphql";
let subscribe = graphqlOperation => _subscribe(api, graphqlOperation);
let subscribeToObservable = graphqlOperation => _subscribe(api, graphqlOperation)|> Wonka.fromObservable;

let extractMessageFrom = event => {
  let event = event->Obj.magic;
  /* get the message value on event and post to ui */
  let message = event##value##data##onCreateMessage##message;
  message;
};
/* setting up like this returns the message on which we can call `setMessage()` */
let subscribeToMessage = graphqlOperation =>
  _subscribe(api, graphqlOperation)
  |> Wonka.fromObservable
  |> Wonka.map((. event) => extractMessageFrom(event));

let subscribeToMessage2 = graphqlOperation => {
   _subscribe(api, graphqlOperation) |> Wonka.fromObservable;
};