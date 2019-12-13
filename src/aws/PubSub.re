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

// type observerLike('value) = {
//   next: event => unit,
//   error: errorValue => unit,
//   complete: unit => unit,
// };

// type observableLike('value) = {
//   subscribe:
//     observerLike('value) => {. [@bs.meth] "unsubscribe": unit => unit},
// };

// don't have to bind to this? it happens on the aws-amplify side i think.
// [@bs.module "@aws-amplify/pubsub"] external pubsub: t = "default";

// [@bs.send]
// external _graphqlSub:
//   (t, Types.graphqlOperation) =>
//   Wonka.observableT(Types.clientResponse('response)) =
//   "graphql";
// [@bs.send]
// external _graphqlSubobservableLike:
//   (t, Types.graphqlOperation) => Wonka.observableT(observableLike('value)) =
//   "graphql";
/* let useSubscriptionResponseToRecord =
       // (parse: Js.Json.t => 'response, result: Types.jsResponse('result)): Types.hookResponse('response) =>
       (parse: 'a => 'response as 'b, result: Types.jsResponse('result))
       : Types.hookResponse('response) => {
     let data =
       result->Types.jsDataGet->Js.Nullable.toOption->Belt.Option.map(parse);
     // // let error: option(GraphqlError.errorJs => GraphqlError.t) => GraphqlError.t =
     // //   result->Types.jsErrorGet->Belt.Option.map(GraphqlError.toJs);
     let error: option(ReactTemplate.GraphqlError.t) =
       result->Types.jsErrorGet->Belt.Option.map(GraphqlError.toJs);
     let fetching = result->Types.fetchingGet;

     let response =
       switch (fetching, data, error) {
       | (true, None, _) => Types.Fetching
       | (false, _, Some(error)) => Error(error)
       | (true, Some(data), _) => Data(data)
       | (false, Some(data), _) => Data(data)
       | (false, None, None) => NotFound
       };

     {fetching, data, error, response};
   }; */
module ClientTypes = {
  /* The record representing the response returned by the client _after_
     it has been converted by urqlClientResponseToReason. */
  type response('response) =
    | Data('response)
    | Error(GraphqlError.t)
    | NotFound;

  type operationResult = {
    data: option(Js.Json.t),
    error: option(GraphqlError.t),
  };

  type clientResponse('response) = {
    data: option('response),
    error: option(GraphqlError.t),
    response: response('response),
  };
};
/* The signature of the Js.t created by calling `.make()` on a `graphql_ppx` module. */
type request('response) = {
  .
  "parse": Js.Json.t => 'response,
  "query": string,
  "variables": Js.Json.t,
};
// type requestJs('response) = {
//   .
//   "parse": Js.Json.t => 'response,
//   "query": string,
//   "variables": Js.Json.t,
// };

// type response('response) =
//   | Fetching
//   | Data('response)
//   | Error(GraphqlError.t)
//   | NotFound;

// type clientResponse('response) = {
//   data: option('response),
//   error: option(GraphqlError.t),
//   response: response('response),
// };
type graphqlOperation = {
  query: string,
  variables: option(Js.Json.t),
};
type resultType = Graphql.OnCreateMessage.t;
// type operationResult('response) = {
//   data: option('response),
//   error: option(GraphqlError.t),
//   response: response('response),
// };
// let clientResponseToReason =
//     (~parse: Js.Json.t => 'response, ~result: ClientTypes.operationResult('response))
//     : ClientTypes.operationResult('response) => {
  let clientResponseToReason =
    (~parse: Js.Json.t => 'response, ~result: ClientTypes.operationResult)
    : ClientTypes.clientResponse('response) => {
  let data = result.data->Belt.Option.map(parse);
  // let error: option(GraphqlError.t) =
  //   result.error->Belt.Option.map(GraphqlError.toJs);
  let error: option(GraphqlError.t) = result.error;

  let response =
    switch (data, error) {
    | (Some(data), _) => ClientTypes.Data(data)
    | (None, Some(error)) => Error(error)
    | (None, None) => NotFound
    };
  Js.log2("clientResponseToReason", response);
  {data, error, response};
};
[@bs.obj]
external createRequest:
  (~query: string, ~variables: Js.Json.t=?, unit) => graphqlOperation =
  "";
[@bs.send]
external executeSubscriptionJs:
  (t, graphqlOperation) => Wonka.observableT(ClientTypes.operationResult) =
  "graphql";

let executeSubscription =
    (~request: request('response))
    : Wonka.Types.sourceT(ClientTypes.clientResponse('response)) => {
  let req =
    createRequest(~query=request##query, ~variables=request##variables, ());
  let parse: Js.Json.t => 'response = request##parse;

  executeSubscriptionJs(api, req)
  |> Wonka.fromObservable
  |> Wonka.map((. result) => clientResponseToReason(~parse, ~result));
};