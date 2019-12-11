type graphqlOperation = {
  query: string,
  variables: Js.Json.t,
};

type executionResult = {
  errors: Js.Nullable.t(array(string)),
  data: option(Js.Json.t),
};
type operation = graphqlOperation => Js.Promise.t(executionResult);
// type subscribe = unit => PubSub.stream;
// type subscription = {state: subscribe};
type subscription = {. "state": {. "subscribe": unit => PubSub.stream}};
// and subscribe = unit => PubSub.stream;
type message = {
  id: option(string),
  message: string,
  createdAt: option(string),
};

type onCreateMessage = {
  __typename: string,
  message: string,
};