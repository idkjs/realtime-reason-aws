type graphqlOperation = {
  query: string,
  variables: Js.Json.t,
};

type executionResult = {
  errors: Js.Nullable.t(array(string)),
  data: option(Js.Json.t),
};
type mutation = graphqlOperation => Js.Promise.t(executionResult);

type message = {
  id: option(string),
  message: string,
  createdAt: option(string),
};