/**
 * The handler type used to type the optional accumulator function
 * returned by useSubscription. handler is a GADT used to support
 * proper type inference for useSubscription.
 */
type handler('acc, 'resp, 'ret) =
  | Handler((option('acc), 'resp) => 'acc): handler('acc, 'resp, 'acc)
  | NoHandler: handler(_, 'resp, 'resp);

/* Arguments passed to useSubscription on the JavaScript side. */
type useSubscriptionArgs = {
  query: string,
  variables: option(Js.Json.t),
};

[@bs.module "urql"]
external useSubscriptionJs:
  (useSubscriptionArgs, option((option('acc), Js.Json.t) => 'acc)) =>
  array(Types.response('ret)) =
  "useSubscription";

/**
 * A function for converting the response to useQuery from the JavaScript
 * representation to a typed Reason record.
 */
let useSubscriptionResponseToRecord =
    // (parse: Js.Json.t => 'response, result: Types.jsResponse('result)): Types.hookResponse('response) =>
    (parse: 'a => 'response as 'b, result: Types.jsResponse('result)): Types.hookResponse('response) =>
     {
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
};

/**
 * The useSubscription hook.
 *
 * Accepts the following arguments:
 *
 * request – a Js.t containing the query and variables corresponding
 * to the GraphQL subscription, and a parse function for decoding the JSON response.
 *
 * handler – an optional function to accumulate subscription responses.
 */;
let useSubscription =
    (
      type acc,
      type resp,
      type ret,
      ~request: Types.request(resp),
      ~handler: handler(acc, resp, ret),
    )
    : Types.hookResponse(ret) => {
  let parse = request.parse;

  let args = {query: request.query, variables: Some(request.variables)};

  React.useMemo3(
    () => {
      let response: Types.hookResponse(ret) =
        switch (handler) {
        | Handler(handlerFn) =>
          useSubscriptionJs(
            args,
            Some((acc, data) => handlerFn(acc, parse(data))),
          )[0]
          |> useSubscriptionResponseToRecord(x=>x)
        | NoHandler =>
          useSubscriptionJs(args, None)[0]
          |> useSubscriptionResponseToRecord(parse)
        };

      response;
    },
    (handler, args, parse),
  );
};