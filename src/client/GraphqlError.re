type message = string;

type sourceLocation = {
  line: int,
  column: int,
};

/* An array of { line, column } locations within the source GraphQL document
   which correspond to this error. */
type locations = array(sourceLocation);

/* Abstract types representing path and astNode.
   path – The JSON-path into the execution response which corresponds to this error.
   astNode - A GraphQL AST node corresponding to this error. */
type path;
type astNode;

/* The source GraphQL document corresponding to this error. */
type source = {
  body: string,
  name: string,
  locationOffset: sourceLocation,
};

/* An array of character offsets within the source GraphQL document which correspond to this error. */
type positions = array(int);

/* The original error thrown from a field resolver during execution. */
type originalError = Js.Exn.t;

/* Extension fields to add to the formatted error. */
type extension;

/* A simple binding to the GraphQL error type exposed by graphql-js. See:
    https://github.com/DefinitelyTyped/DefinitelyTyped/blob/master/types/graphql/error/GraphQLError.d.ts.
   */
type t = {
  message: option(message),
  locations: option(locations),
  path: option(array(path)),
  nodes: option(array(astNode)),
  source: option(source),
  positions: option(positions),
  originalError: option(originalError),
  extensions: option(Js.Dict.t(extension)),
};
// type errorJs ={
//   message: Js.Nullable.t(message),
//   locations: Js.Nullable.t(locations),
//   path: Js.Nullable.t(array(path)),
//   nodes: Js.Nullable.t(array(astNode)),
//   source: Js.Nullable.t(source),
//   positions: Js.Nullable.t(positions),
//   originalError: Js.Nullable.t(originalError),
//   extensions: Js.Nullable.t(Js.Dict.t(extension)),
// };

// let toJs: errorJs => t =
//   error => {
//     message: Js.Nullable.toOption(error.message),
//     locations: Js.Nullable.toOption(error.locations),
//     path: Js.Nullable.toOption(error.path),
//     nodes: Js.Nullable.toOption(error.nodes),
//     source: Js.Nullable.toOption(error.source),
//     positions: Js.Nullable.toOption(error.positions),
//     originalError: Js.Nullable.toOption(error.originalError),
//     extensions: Js.Nullable.toOption(error.extensions),
//   };
type errorJs = {
  message: option(message),
  locations: option(locations),
  path: option(array(path)),
  nodes: option(array(astNode)),
  source: option(source),
  positions: option(positions),
  originalError: option(originalError),
  extensions: option(Js.Dict.t(extension)),
};
let toJs: errorJs => t =
  error => {
    message: error.message,
    locations: error.locations,
    path: error.path,
    nodes: error.nodes,
    source: error.source,
    positions: error.positions,
    originalError: error.originalError,
    extensions: error.extensions,
  };
