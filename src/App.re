[@bs.val]
external jsonStringify: ('a, Js.Nullable.t(unit), int) => string =
  "JSON.stringify";
let getInputValue = (e): string => ReactEvent.Form.target(e)##value;
// let logo = [%bs.raw {|require('./logo.svg')|}];
[%bs.raw {|require('./App.css')|}];
[%bs.raw {|require('bootstrap/dist/css/bootstrap.min.css')|}];
[%bs.raw {|require("@aws-amplify/pubsub")|}];

Amplify.configure(AwsExports.config);
API.configure(AwsExports.config);

open Types;

[@react.component]
let make = () => {
  // let (message, setMessage) = React.useState(() => None);
  let (value, setValue) = React.useState(() => "");

  let handleSubmit = event => {
    let time = Js.Date.now();
    let value = "RE: " ++ time->Js.Date.fromFloat->Js.Date.toLocaleString;
    setValue(_ => value);
    let _ = ReactEvent.Form.preventDefault(event);
    let _ = ReactEvent.Form.stopPropagation(event);
    let message = {"id": None, "message": value, "createdAt": None};
    Js.log2("Message: ", message);
    let mutationRequest = Graphql.CreateMessage.make(~input=message, ());
    let graphqlOperation: Types.graphqlOperation = {
      query: mutationRequest##query,
      variables: mutationRequest##variables,
    };
    ();
    API.mutate(graphqlOperation)
    |> Js.Promise.then_(response =>
         {
           Js.log2("reason_broadcaster_mutation", response.data);
         }
         |> Js.Promise.resolve
       );
  };

  let handleChange = e => {
    let value = e |> getInputValue;
    setValue(_ => value);
  };
  <div className="container">
    // <img src=logo className="App-logo" alt="logo" />

      <div className="jumbotron jumbotron-fluid p-0">
        <h2 className="center"> "Reason Broadcaster"->React.string </h2>
      </div>
      <br />
      <form onSubmit={e => handleSubmit(e) |> ignore}>
        <div className="form-group">
          <input
            className="form-control form-control-lg"
            type_="text"
            value
            onChange={e => handleChange(e)}
          />
          <input
            id="button"
            type_="submit"
            value="Submit"
            className="btn btn-primary"
          />
        </div>
      </form>
      <br />
    </div>;
  // {display
  //    ? <div className="container">
  //        <div className="card bg-success">
  //          <h3 className="card-text text-white p-2">
  //            message->React.string
  //          </h3>
  //        </div>
  //      </div>
  //    : React.null}
  // {switch (message) {
  //  | Some(message) =>
  //    <div className="container">
  //      <div className="card bg-success">
  //        <h3 className="card-text text-white p-2">
  //          message->React.string
  //        </h3>
  //      </div>
  //    </div>
  //  | None => React.null
  //  }}
};
let default = make;