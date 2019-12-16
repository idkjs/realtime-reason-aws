open Utils;
[%bs.raw {|require('./App.css')|}];
[%bs.raw {|require('bootstrap/dist/css/bootstrap.min.css')|}];
// external castToStr: 'a => string = "%identity";
let logo = [%bs.raw {|require('./logo.svg')|}];
Amplify.configure(AwsExports.config);
API.configure(AwsExports.config);

[@react.component]
let make = () => {
  let (message, setMessage) = React.useState(() => None);
  let (value, setValue) = React.useState(() => "");

  let handleSubmit = event => {
    /* create a message using a timestamp so we dont have to keep putting in a message to test */
    let time = Js.Date.now();
    let value = "RE: " ++ time->Js.Date.fromFloat->Js.Date.toLocaleString;
    setValue(_ => value);
    let _ = ReactEvent.Form.preventDefault(event);
    let _ = ReactEvent.Form.stopPropagation(event);
    let message = {"id": None, "message": value, "createdAt": None};
    let mutationRequest = Graphql.CreateMessage.make(~input=message, ());
    let graphqlOperation: Types.graphqlOperation = {
      query: mutationRequest##query,
      variables: Some(mutationRequest##variables),
    };
    API.mutate(graphqlOperation)
    |> Js.Promise.then_(response => {
         Js.log2("reason_broadcaster_mutation", response)
         |> Js.Promise.resolve
       });
  };

  React.useEffect(() => {
    let subRequest = Graphql.OnCreateMessage.make();
    let graphqlOperation: Types.graphqlOperation = {
      query: subRequest##query,
      variables: Some(subRequest##variables),
    };

    let sub =
      API.subscribeToObservable(graphqlOperation)
      // |> Wonka.share
      |> Wonka.forEach((. event) => {
           let message = API.extractMessageFrom(event);
           // Js.log2("subscribeToMessage2_event", event);
           setMessage(_ => Some(message));
         });

    Some(() => sub);
  });
  let handleChange = e => {
    let value = e |> getInputValue;
    setValue(_ => value);
  };
  <div className="App">
    <img src=logo className="App-logo" alt="logo" />
    <div className="container">
      <div className="jumbotron jumbotron-fluid p-0">
        <h2 className="center"> "Reason Broadcaster"->React.string </h2>
        {switch (message) {
         | Some(message) =>
           <div className="container">
             <div className="card bg-success">
               <h2 className="center">
                 "Reason WSS Sub Response"->React.string
               </h2>
               <h3 className="card-text text-white p-2">
                 message->React.string
               </h3>
             </div>
           </div>
         | None => React.null
         }}
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
    </div>
  </div>;
};
let default = make;