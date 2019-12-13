[@bs.obj]
external createRequest:
  (~query: string, ~variables: Js.Json.t=?, unit) => ApiTypes.graphqlRequest =
  "";