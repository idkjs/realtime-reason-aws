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
// type objectWithCallback = {
//   closed: option(bool),
//   next: option(event => unit),
//   error: option(errorValue => unit),
//   complete: option(unit=>unit),
// };
type objectWithCallback('event) = {
  closed: option(bool),
  next: option(event => unit),
  error: option(errorValue => unit),
  complete: option(unit => unit),
};
type observerLike('value) = {
  next: 'value => unit,
  error: Js.Exn.t => unit,
  complete: unit => unit,
};

type observableLike('value) = {
  subscribe:
    observerLike('value) => {. [@bs.meth] "unsubscribe": unit => unit},
};
type eventCallback = {
  closed: option(bool),
  next: option(event => unit),
  error: option(errorValue => unit),
  complete: option(unit => unit),
};
let objectWithCallback: objectWithCallback('event) = {
  next: Some(event => Js.log2("event", event)),
  error: Some(errorValue => Js.log(errorValue)),
  closed: Some(true),
  complete: Some(_ => Js.log("complete")),
};

[@bs.module "@aws-amplify/pubsub"] external pubsub: t = "default";

[@bs.send]
external _graphqlSubWonka:
  (t, Types.graphqlOperation) => Wonka.observableT('a) =
  "graphql";
[@bs.send]
external _graphqlSubCb:
  (t, Types.graphqlOperation) => Wonka.observableT(subscriptionObserver) =
  "graphql";
[@bs.send]
external _graphqlSubUrql:
  (t, Types.graphqlOperation) =>
  Wonka.observableT(Types.clientResponse('response)) =
  "graphql";
let mutate: Types.operation =
  graphqlOperation => {
    _graphql(api, graphqlOperation);
  };
let objectWithCallback: objectWithCallback('event) = {
  next: Some(event => Js.log2("event", event)),
  error: Some(errorValue => Js.log(errorValue)),
  closed: Some(true),
  complete: Some(_ => Js.log("complete")),
};
let graphqlSubCb = graphqlOperation => {
  _graphqlSubCb(api, graphqlOperation);
};
let graphqlSubUrql = graphqlOperation => {
  _graphqlSubUrql(api, graphqlOperation);
};
let subWithWonka = graphqlOperation => {
  _graphqlSubWonka(api, graphqlOperation);
};

let clientResponseToReason =
    (~parse: Js.Json.t => 'response, ~result: Types.operationResult)
    : Types.clientResponse('response) => {
  let data = result.data->Js.Nullable.toOption->Belt.Option.map(parse);
  let error = result.error->Js.Nullable.toOption;

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
  (~client: t, ~query: Types.graphqlOperation, unit) =>
  Wonka.Types.sourceT(Types.operationResult) =
  "graphql";
let executeQuery =
    (~request: Types.request('response), ())
    : Wonka.Types.sourceT(Types.clientResponse('response)) => {
  let req: Types.graphqlOperation = {
    query: request##query,
    variables: Some(request##variables),
  };
  // let req =
  //   Types.createRequest(
  //     ~query=request##query,
  //     ~variables=request##variables,
  //     (),
  //   );
  let parse = request##parse;

  _graphql(~client=api, ~query=req, ())
  |> Wonka.map((. result) => clientResponseToReason(~parse, ~result));
};

[@bs.send]
external executeSubscriptionJs:
  (t, Types.graphqlOperation) => Wonka.Types.sourceT(Types.operationResult) =
  "graphql";

let executeSubscription =
    (~request: Types.request('response))
    : Wonka.Types.sourceT(Types.clientResponse('response)) => {
  let req: Types.graphqlOperation = {
    query: request##query,
    variables: Some(request##variables),
  };
  let parse = request##parse;

  executeSubscriptionJs(api, req)
  |> Wonka.map((. result) => clientResponseToReason(~parse, ~result));
};