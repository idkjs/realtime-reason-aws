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
  error: option(GraphqlError.t),
};

type response('response) =
  | Fetching
  | Data('response)
  | Error(GraphqlError.t)
  | NotFound;

type clientResponse('response) = {
  data: option('response),
  error: option(GraphqlError.t),
  response: response('response),
};
type hookResponse('ret) = {
  fetching: bool,
  data: option('ret),
  error: option(GraphqlError.t),
  response: response('ret),
};
[@bs.deriving abstract]
type jsResponse('response) = {
  fetching: bool,
  [@bs.as "data"]
  jsData: Js.Nullable.t('response),
  [@bs.optional] [@bs.as "error"]
  jsError: GraphqlError.errorJs,
};
// type clientResponse('response) =
//   ApiClient.ClientTypes.clientResponse('response) = {
//     data: option('response),
//     error: option(GraphqlError.t),
//     response: ApiClient.ClientTypes.response('response),
//   };
type executionResult = {
  errors: Js.Nullable.t(array(string)),
  data: option(Js.Json.t),
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
// type event = {
//   provider: Js.Json.t,
//   value,
// }
// and value = {data: onCreateMessage};
// type errorValue = {message: string};
type operation = graphqlOperation => Js.Promise.t(executionResult);

type message = {
  id: option(string),
  message: string,
  createdAt: option(string),
};