type graphqlOperation = {
  query: string,
  variables: Js.Json.t,
};

type executionResult = {
  errors: Js.Nullable.t(array(string)),
  data: option(Js.Json.t),
};
type onCreateMessage = {
  __typename: string,
  message: string,
};
// type event = {
//   provider: Js.Json.t,
//   value,
// }
// and value = {data: onCreateMessage};
// type errorValue = {message: string};
// type subscriptionObserver = {
//   closed: bool,
//   next: event => unit,
//   error: errorValue => unit,
//   complete: unit,
// };
type operation = graphqlOperation => Js.Promise.t(executionResult);

// type subscribe = unit => PubSub.stream;
// type subscription = {state: subscribe};
// type subscription = {. "state": {. "subscribe": unit => PubSub.stream}};
// and subscribe = unit => PubSub.stream;
type message = {
  id: option(string),
  message: string,
  createdAt: option(string),
};

