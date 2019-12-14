
[@bs.val]
external jsonStringify: ('a, Js.Nullable.t(unit), int) => string =
  "JSON.stringify";
let getInputValue = (e): string => ReactEvent.Form.target(e)##value;
[%bs.raw {|require('./App.css')|}];
[%bs.raw {|require('bootstrap/dist/css/bootstrap.min.css')|}];

Amplify.configure(AwsExports.config);
API.configure(AwsExports.config);

[@react.component]
let make = () => {
  let (message, setMessage) = React.useState(() => None);
  let (value, setValue) = React.useState(() => "");

  let handleSubmit = event => {
    let time = Js.Date.now();
    let value = "RE: " ++ time->Js.Date.fromFloat->Js.Date.toLocaleString;
    setValue(_ => value);
    let _ = ReactEvent.Form.preventDefault(event);
    let _ = ReactEvent.Form.stopPropagation(event);
    let message = {"id": None, "message": value, "createdAt": None};
    let mutationRequest = Graphql.CreateMessage.make(~input=message, ());
    let graphqlOperation: API.graphqlOperation = {
      query: mutationRequest##query,
      variables: Some(mutationRequest##variables),
    };
    API.mutate(graphqlOperation)
    |> Js.Promise.then_(response => {
         Js.log2("reason_broadcaster_mutation", response)
         |> Js.Promise.resolve
       });
  };

  let handleSubscriptionEvent = event => {
    /* use Obj.magic to change time, otherwise code in Wonka.subcribe breaks. */
    let event = event->Obj.magic;
    /* get the message value on event and post to ui */
    let message = event##value##data##onCreateMessage##message;
    setMessage(_ => Some(message));
  };
  React.useEffect0(() => {
    let subRequest = Graphql.OnCreateMessage.make();
    let graphqlOperation: API.graphqlOperation = {
      query: subRequest##query,
      variables: Some(subRequest##variables),
    };
    let wonkaObservableT = API.subscriptionSink(graphqlOperation);
    let wonkaSubscriptionT =
      wonkaObservableT
      |> Wonka.fromObservable
      |> Wonka.subscribe((. event) => {
           handleSubscriptionEvent(event);
           Js.log2("wonkaSubscriptionT_event", event);
         });
    Some(() => wonkaSubscriptionT.unsubscribe());
    // let subSink = API.subObsLike(graphqlOperation);
    // |> Wonka.map((. event) =>
    //      switch (event) {
    //      | event => {
    //          next: event => {
    //            Js.log2(
    //              "Subscription: ",
    //              Utils.jsonStringify(event.value.data, Js.Nullable.null, 2),
    //            );
    //            Js.log2(
    //              "EVENT: ",
    //              Utils.jsonStringify(event, Js.Nullable.null, 2),
    //            );
    //            let message = event.value.data.message;
    //            setMessage(_ => Some(message));
    //          },
    //          error: errorValue => Js.log(errorValue),
    //          complete: _ => Js.log("COMPLETE"),
    //        }
    //      }
    //    );
    // |> Wonka.map((. observableLikeValue) => observableLikeValue)
    // |> Wonka.subscribe(
    //      (. API.observableLikeValue) =>
    //        {
    //          next: event => {
    //            Js.log2(
    //              "Subscription: ",
    //              Utils.jsonStringify(event.value.data, Js.Nullable.null, 2),
    //            );
    //            Js.log2(
    //              "EVENT: ",
    //              Utils.jsonStringify(event, Js.Nullable.null, 2),
    //            );
    //            let message = event.value.data.message;
    //            setMessage(_ => Some(message));
    //          },
    //          error: errorValue => Js.log(errorValue),
    //          complete: _ => Js.log("COMPLETE"),
    //        },
    //    );
    // let _ =
    //   sub
    //   |> Wonka.subscribe((. x) => {
    //        Js.log2(
    //          "subObsLike_",
    //          x,
    //          // let data = x;
    //        )
    //      });
    //     let sub = API.subWithWonka2(graphqlOperation);
    //     let _ =
    //       sub |> Wonka.fromObservable |> Wonka.subscribe((. x) =>{
    //         Js.log2("subWithWonka2_RAQ", x);
    // // let data = x;
    //         });
    // API.subWithWonka2(graphqlOperation)
    // |> Wonka.fromObservable((. result) => Js.log(result));
    // Some(() => sub.unsubscribe());
    // None;
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
           <h2 className="center"> "Reason WSS Sub Response"->React.string </h2>
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