open Utils;
[%bs.raw {|require('./App.css')|}];
[%bs.raw {|require('bootstrap/dist/css/bootstrap.min.css')|}];

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

  let extractMessageFrom = event => {
    /* use Obj.magic to change time, otherwise code in Wonka.subcribe breaks. */
    let event = event->Obj.magic;
    /* get the message value on event and post to ui */
    let message = event##value##data##onCreateMessage##message;
    message;
  };
  React.useEffect0(() => {
    let subRequest = Graphql.OnCreateMessage.make();
    let graphqlOperation: Types.graphqlOperation = {
      query: subRequest##query,
      variables: Some(subRequest##variables),
    };
    /* The observer's type is:
       `Wonka.observableT(
          ReactTemplate.Types.observableLike(ReactTemplate.Types.observerLike('a))
          )`
           */
    let observer = API.subscribe(graphqlOperation);
    let subscription =
      observer
      |> Wonka.fromObservable
      |> Wonka.subscribe((. event) => {
            let message = extractMessageFrom(event);
           setMessage(_ => Some(message));
           Js.log2("subscription_event", message);
         });
    Some(() => subscription.unsubscribe());
  });
  let handleChange = e => {
    let value = e |> getInputValue;
    setValue(_ => value);
  };
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
  </div>;
};
let default = make;