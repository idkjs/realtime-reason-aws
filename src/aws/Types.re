type graphqlOperation = {
  query: string,
  variables: option(Js.Json.t),
};
type graphqlRequest = {
  key: int,
  query: string,
  variables: option(Js.Json.t),
};
// type graphqlOperation = {
//   query: string,
//   variables: Js.Json.t,
// }
// [@bs.obj]
// external createRequest:
//   (~query: string, ~variables: Js.Json.t=?, unit) => graphqlOperation =
//   "";
/* The signature of the Js.t created by calling `.make()` on a `graphql_ppx` module. */
type request('response) = {
  parse: Js.Json.t => 'response,
  query: string,
  variables: Js.Json.t,
};
type requestJs('response) = {
  .
  "parse": Js.Json.t => 'response,
  "query": string,
  "variables": Js.Json.t,
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
  data: option(Js.Json.t),
  error: option(string),
};

type response('response) =
  | Fetching
  | Data('response)
  | Error(string)
  | NotFound;

type clientResponse('response) = {
  data: option('response),
  error: option(string),
  response: response('response),
};

type onCreateMessage = {
  __typename: string,
  message: string,
};
module OnCreateMessage = [%graphql
  {|
    subscription onCreateMessage {
      onCreateMessage {
      __typename
      message
      }
    }
|}
];
type event = {
  provider: Js.Json.t,
  value,
}
and value = {data: onCreateMessage};
type errorValue = {message: string};

type observerLike('value) = {
  next: event => unit,
  error: errorValue => unit,
  complete: unit => unit,
};

type observableLike('value) = {
  subscribe:
    observerLike('value) => {. [@bs.meth] "unsubscribe": unit => unit},
};
// type subscription = graphqlOperation => Wonka.observableT(observableLike(observerLike(value)));

type message = {
  id: option(string),
  message: string,
  createdAt: option(string),
};