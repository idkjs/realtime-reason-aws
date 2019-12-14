type t;
[@bs.module "@aws-amplify/api"] external api: t = "default";
[@bs.send] external configure: (t, AwsExports.t) => unit = "configure";
let configure = config => configure(api, config);

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
// type objectWithCallback = {
//   closed: option(bool),
//   next: option(event => unit),
//   error: option(errorValue => unit),
//   complete: option(unit=>unit),
// };
// type objectWithCallback('event) = {
//   closed: option(bool),
//   next: option(event => unit),
//   error: option(errorValue => unit),
//   complete: option(unit => unit),
// };
// type observerLike('value) = {
//   next: event => unit,
//   error: errorValue => unit,
//   complete: unit => unit,
// };
// type observableLike('value) = {
//   subscribe:
//     observerLike('value) => {. [@bs.meth] "unsubscribe": unit => unit},
// };

// type eventCallback = {
//   closed: option(bool),
//   next: option(event => unit),
//   error: option(errorValue => unit),
//   complete: option(unit => unit),
// };
// let objectWithCallback: objectWithCallback('event) = {
//   next: Some(event => Js.log2("event", event)),
//   error: Some(errorValue => Js.log(errorValue)),
//   closed: Some(true),
//   complete: Some(_ => Js.log("complete")),
// };
let observerLikeEvent: Types.observerLike('event) = {
  next: event => Js.log2("event", event),
  error: errorValue => Js.log(errorValue),
  complete: _ => Js.log("complete"),
};
let listener: Types.observerLike('event) = {
  next: event => {
    Js.log2(
      "Subscription: ",
      Utils.jsonStringify(event.value.data, Js.Nullable.null, 2),
    );
    Js.log2("EVENT: ", Utils.jsonStringify(event, Js.Nullable.null, 2));
    let message = event.value.data.message;
    Js.log2("MESSAGE: ", Utils.jsonStringify(message, Js.Nullable.null, 2));
    // setMessage(_ => Some(message));
  },
  error: errorValue => Js.log(errorValue),
  complete: _ => Js.log("COMPLETE"),
};

// let objectWithCallback: objectWithCallback('event) = {
//   next: Some(event => Js.log2("event", event)),
//   error: Some(errorValue => Js.log(errorValue)),
//   closed: Some(true),
//   complete: Some(_ => Js.log("complete")),
// };
// don't have to bind to this? it happens on the aws-amplify side i think.
// [@bs.module "@aws-amplify/pubsub"] external pubsub: t = "default";
type executionResult = {
  errors: option(array(string)),
  data: option(Js.Json.t),
};
type graphqlOperation = {
  query: string,
  variables: option(Js.Json.t),
};
[@bs.send]
external _graphql: (t, graphqlOperation) => Js.Promise.t(executionResult) =
  "graphql";
[@bs.send]
external _graphqlSubWonka:
  (t, graphqlOperation) => Wonka.observableT(Types.clientResponse('response)) =
  "graphql";

[@bs.send]
external _graphqlSubUrql:
  (t, graphqlOperation) => Wonka.observableT(Types.clientResponse('response)) =
  "graphql";

type operation = graphqlOperation => Js.Promise.t(executionResult);
// type subscription =
//   graphqlOperation =>
//   Wonka.observableT(Types.observableLike('event));
let mutate: operation =
  graphqlOperation => {
    _graphql(api, graphqlOperation);
  };
[@bs.send]
external _graphqlObsLike:
  (t, graphqlOperation) =>
  Wonka.observableT(Types.observableLike(Types.observerLike('value))) =
  "graphql";
  let subscriptionSink = graphqlOperation =>
    _graphqlObsLike(api, graphqlOperation);
let subObsLike = graphqlOperation => {
  _graphqlObsLike(api, graphqlOperation)
  |> Wonka.fromObservable(_)
  |> Wonka.map((. observableLikeValue) => observableLikeValue);
};

let clientResponseToReason =
    (~parse: Js.Json.t => 'response, ~result: Types.operationResult)
    : Types.clientResponse('response) => {
  let data = result.data->Belt.Option.map(parse);
  let error = result.error;

  let response =
    switch (data, error) {
    | (Some(data), _) => Types.Data(data)
    | (None, Some(error)) => Error(error)
    | (None, None) => NotFound
    };

  {data, error, response};
};
[@bs.send]
external _graphql:
  (~client: t, ~query: graphqlOperation, unit) =>
  Wonka.Types.sourceT(Types.operationResult) =
  "graphql";
let executeQuery =
    (~request: Types.request('response), ())
    : Wonka.Types.sourceT(Types.clientResponse('response)) => {
  let req: graphqlOperation = {
    query: request.query,
    variables: Some(request.variables),
  };
  // let req =
  //   Types.createRequest(
  //     ~query=request##query,
  //     ~variables=request##variables,
  //     (),
  //   );
  let parse = request.parse;

  _graphql(~client=api, ~query=req, ())
  |> Wonka.map((. result) => clientResponseToReason(~parse, ~result));
};

[@bs.send]
external executeSubscriptionJs:
  (t, graphqlOperation) => Wonka.Types.sourceT(Types.operationResult) =
  "graphql";

let executeSubscription =
    (~request: Types.request('response))
    : Wonka.Types.sourceT(Types.clientResponse('response)) => {
  let req: graphqlOperation = {
    query: request.query,
    variables: Some(request.variables),
  };
  let parse = request.parse;

  executeSubscriptionJs(api, req)
  |> Wonka.map((. result) => clientResponseToReason(~parse, ~result));
};