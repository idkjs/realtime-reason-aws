/* A message describing the Error for debugging purposes. */
type t;

module ClientTypes = {
  type executionResult = {
    errors: Js.Nullable.t(array(GraphqlError.t)),
    data: Js.Nullable.t(Js.Json.t),
  };
  /* OperationType for the active operation.
     Use with operationTypeToJs for proper conversion to strings.
     see:https://github.com/aws-amplify/amplify-js/blob/86597db2b0704d7aff5b612557536142b82e1731/packages/api/src/API.ts#L337 */

  // this version has higher runtime cost per [@yawaramin](https://dev.to/yawaramin/inlined-values-in-bucklescript-247c)
  module OperationTypeHigherRuntimeCost: {
    type t;

    let subscription: t;
    let query: t;
    let mutation: t;
  } = {
    type t = string;

    let subscription = "subscription";
    let query = "query";
    let mutation = "mutation";
  };
  // see: [Inlined values in BuckleScript](https://dev.to/yawaramin/inlined-values-in-bucklescript-247c)
  module OperationType: {
    type t;

    [@bs.inline "subscription"]
    let subscription: t;
    [@bs.inline "mutation"]
    let mutation: t;
    [@bs.inline "query"]
    let query: t;
  } = {
    type t = string;
    [@bs.inline]
    let subscription = "subscription";
    [@bs.inline]
    let mutation = "mutation";
    [@bs.inline]
    let query = "query";
  };
  type operationResult = {
    operation: OperationType.t,
    data: Js.Nullable.t(Js.Json.t),
    error: Js.Nullable.t(GraphqlError.t),
  };
  /* The record representing the response returned by the client _after_
     it has been converted by urqlClientResponseToReason. */
  type response('response) =
    | Data('response)
    | Error(GraphqlError.t)
    | NotFound;

  type clientResponse('response) = {
    data: option('response),
    error: option(GraphqlError.t),
    response: response('response),
  };
  // type operationType = [
  //   | [@bs.string "subscription"] `Subscription
  //   | [@bs.string "query"] `Query
  //   | [@bs.string "mutation"] `Mutation
  // ];

  // type response('response) =
  //   | Data('response)
  //   | Error(GraphqlError.t)
  //   | NotFound;

  // type clientResponse('response) = {
  //   data: option('response),
  //   error: option(GraphqlError.t),
  //   response: response('response),
  // };
  // type graphqlOperation = {
  //   query: string,
  //   variables: Js.Json.t,
  // };
  // same as above but rebranded to not mix the operations for now.
  type subscriptionOperation = {
    query: string,
    variables: Js.Json.t,
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

  type subscriptionForwarder =
    subscriptionOperation => observableLike(executionResult);
};

[@bs.module "@aws-amplify/api"] external client: t = "default";
[@bs.send] external configure: (t, AwsExports.t) => unit = "configure";
let make = (~config: AwsExports.t) => {
  configure(client, config);
  client;
};
let clientResponseToReason =
    (~parse: Js.Json.t => 'response, ~result: ClientTypes.operationResult)
    : ClientTypes.clientResponse('response) => {
  let data = result.data->Js.Nullable.toOption->Belt.Option.map(parse);
  let error = result.error->Js.Nullable.toOption;

  let response =
    switch (data, error) {
    | (Some(data), _) => ClientTypes.Data(data)
    | (None, Some(error)) => Error(error)
    | (None, None) => NotFound
    };

  {data, error, response};
};
/* Converts @baransu/graphql_ppx_re function to reason record */
// let ppxToReason: 'a => ApiTypes.request('response) =
//   queryJst => {
//     parse: queryJst##parse,
//     query: queryJst##query,
//     variables: queryJst##variables,
//   };
/* Execution methods on the client. These allow you to imperatively execute GraphQL
   operations outside of components or hooks. */
[@bs.send]
external _graphql:
  (~client: t, ~query: ApiTypes.graphqlRequest, unit) =>
  Wonka.Types.sourceT(ClientTypes.operationResult) =
  "graphql";

let executeQuery =
    (~client: t, ~request: ApiTypes.request('response), ())
    : Wonka.Types.sourceT(ClientTypes.clientResponse('response)) => {
  // let request =request->ppxToReason;
  let req =
    ApiRequest.createRequest(
      ~query=request##query,
      ~variables=request##variables,
      (),
    );
  let parse = request##parse;

  _graphql(~client, ~query=req, ())
  |> Wonka.map((. result) => clientResponseToReason(~parse, ~result));
};

[@bs.send]
external executeSubscriptionJs:
  (~client: t, ~subscription: ApiTypes.graphqlRequest, unit) =>
  Wonka.Types.sourceT(ClientTypes.operationResult) =
  "graphql";

let executeSubscription =
    (~client: t, ~request: ApiTypes.request('response), ())
    : Wonka.Types.sourceT(ClientTypes.clientResponse('response)) => {
  let req =
    ApiRequest.createRequest(
      ~query=request##query,
      ~variables=request##variables,
      (),
    );
  let parse = request##parse;

  executeSubscriptionJs(~client, ~subscription=req, ())
  |> Wonka.map((. result) => clientResponseToReason(~parse, ~result));
};