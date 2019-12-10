type t;
[@bs.module "@aws-amplify/api"] external api: t = "default";
[@bs.send] external configure: (t, AwsExports.t) => unit = "configure";
let configure = config => configure(api, config);

[@bs.send]
external _graphql:
  (t, Types.graphqlOperation) => Js.Promise.t(Types.executionResult) =
  "graphql";
let mutate: Types.mutation =
  graphqlOperation => {
    _graphql(api, graphqlOperation);
  };
// [@bs.send]
// external _graphql:
//   (t, Types.graphqlOperation) =>
//   Wonka_types.sourceT(
//     ReasonUrql.Client.ClientTypes.response(Types.executionResult),
//   ) =
//   "graphql";

// [@bs.send]
// external subscription:
//   (t, Types.graphqlOperation) =>
//   Wonka_types.sourceT(ReasonUrql.Client.ClientTypes.response('a)) =
//   "graphql";
// let subscription = graphqlOperation => subscription(api, graphqlOperation);