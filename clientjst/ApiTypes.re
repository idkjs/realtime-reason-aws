type graphqlOperation = {
  query: string,
  variables: Js.Json.t,
};
type graphqlRequest = {
  query: string,
  variables: option(Js.Json.t),
};
/* The response variant wraps the parsed result of executing a GraphQL operation. */

type response('response) =
  | Data('response)
  | Error(GraphqlError.t)
  | NotFound;

/* The GraphQL request object.
   Consists of a query, a unique key for the event, and, optionally, variables. */
type clientResponse('response) = {
  data: option('response),
  error: option(GraphqlError.t),
  response: response('response),
};

/* The signature of the Js.t created by calling `.make()` on a `graphql_ppx` module. */
// type request('response) = {
//   parse: Js.Json.t => 'response,
//   query: string,
//   variables: Js.Json.t,
// };
type request('response) = {
  .
  "parse": Js.Json.t => 'response,
  "query": string,
  "variables": Js.Json.t,
};
type hookResponse('ret) = {
  fetching: bool,
  data: option('ret),
  error: option(GraphqlError.t),
  response: response('ret),
};

type jsResponse('response) = {
  fetching: bool,
  data: Js.Nullable.t('response),
  error: option(GraphqlError.t),
};

type graphqlDefinition('parseResult, 'composeReturnType, 'hookReturnType) = (
  // `parse`
  Js.Json.t => 'parseResult,
  // `query`
  string,
  // `composeVariables`
  (Js.Json.t => 'composeReturnType) => 'hookReturnType,
);