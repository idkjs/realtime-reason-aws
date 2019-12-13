open GqlClient;
open Client;
let client = make(~config=AwsExports.config);
// let main = () => {
//   let time = Js.Date.now();
//   let value = "RE: " ++ time->Js.Date.fromFloat->Js.Date.toLocaleString;
// setValue(_ => value);
// let _ = ReactEvent.Form.preventDefault(event);
// let _ = ReactEvent.Form.stopPropagation(event);
//   let message = {"id": None, "message": value, "createdAt": None};
//   Js.log2("Message: ", message);
//   let mutationRequest = Graphql.CreateMessage.make(~input=message, ());
//   let graphqlOperation: Types.graphqlOperation = {
//     query: mutationRequest##query,
//     variables: mutationRequest##variables,
//   };
//   let request = Graphql.ListMessages.make();
//   let query = Client.executeQuery(~client, ~request, ());
//   let request = Graphql.CreateMessage.make(~input=message, ());
//   executeQuery(~client, ~request, ())
//   |> Js.Promise.then_(response =>
//        {
//          Js.log2("reason_broadcaster_mutation", response.data);
//        }
//        |> Js.Promise.resolve
//      );
// };
type state = {
  query: string,
  // mutation: string,
};

type action =
  | SetQuery(string)
  | ClearQuery;
// | SetMutation(string)
// | ClearMutation;

let initialState = {
  query: "",
  // , mutation: ""
};
// let ppxToReason: 'a => GqlClient.ApiTypes.request('response) =
//   queryJst => {
//     parse: Js.Json.t => 'response,
//     query: queryJst##query,
//     variables: queryJst##variables,
//   };
let queryRequest = Graphql.ListMessages.make();
let testQuery = () =>
  executeQuery(~client, ~request=queryRequest, ())
  |> Wonka.subscribe((. data) =>
       switch (data.response) {
       | Data(d) =>
         switch (Js.Json.stringifyAny(d)) {
         | Some(s) => Js.log2("testQuery", s)
         | None => ()
         }
       | Error(e) =>
         switch (Js.Json.stringifyAny(e)) {
         | Some(s) => Js.log2("testQuery", s)
         | None => ()
         }
       | _ => ()
       }
     );

// let queryRequest = queryRequestJst->ppxToReason;
[@react.component]
let make = () => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | SetQuery(query) => {...state, query}
        | ClearQuery => {...state, query: ""}
        // | SetMutation(mutation) => {...state, mutation}
        // | ClearMutation => {...state, mutation: ""}
        },
      initialState,
    );

  let executeQuery = () =>
    executeQuery(~client, ~request=queryRequest, ())
    |> Wonka.subscribe((. data) =>
         switch (data.response) {
         | Data(d) =>
           switch (Js.Json.stringifyAny(d)) {
           | Some(s) => dispatch(SetQuery(s))
           | None => ()
           }
         | Error(e) =>
           switch (Js.Json.stringifyAny(e)) {
           | Some(s) => dispatch(SetQuery(s))
           | None => ()
           }
         | _ => ()
         }
       );

  <div className="container">

      <div className="jumbotron jumbotron-fluid p-0">
        <h2 className="center"> "CheckConsole"->React.string </h2>
      </div>
      <section>
        <button
          className="btn btn-primary"
          onClick={_event => executeQuery() |> ignore}>
          "Execute Query"->React.string
        </button>
      </section>
    </div>;
    // {switch (String.length(state.query)) {
    //  | 0 => React.null
    //  | _ =>
    //    <div className="container">
    //      <div className="card bg-success">
    //        <h3 className="card-text text-white p-2">
    //          "Result"->React.string
    //        </h3>
    //      </div>
    //      <div> state.query->React.string </div>
    //      <button
    //        className="btn btn-primary"
    //        onClick={_event => dispatch(ClearQuery)}>
    //        "Clear Query"->React.string
    //      </button>
    //    </div>
    //  }}
};
let default = make;