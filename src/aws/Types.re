type executionResult = {
  errors: option(array(string)),
  data: option(Js.Json.t),
};
/* https://github.com/aws-amplify/amplify-js/blob/867412030de57fd74078b609252de6f7f81ad331/packages/api/src/API.ts#L31-L34 */
type graphqlOperation = {
  query: string,
  variables: option(Js.Json.t),
};

type operation = graphqlOperation => Js.Promise.t(executionResult);

type onCreateMessage = {
  __typename: string,
  message: string,
};

type event = {value}
and value = {data: onCreateMessage};

type errorValue = {message: string};

type observerLike('event) = {
  next: event => unit,
  error: errorValue => unit,
  complete: unit => unit,
};

type observableLike('value) = {
  subscribe:
    observerLike('value) => {. [@bs.meth] "unsubscribe": unit => unit},
};

type message = {
  id: option(string),
  message: string,
  createdAt: option(string),
};