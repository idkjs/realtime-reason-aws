external castToJst: 'a => Js.t('a) = "%identity";
"JSON.stringify";
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
PubSub.configurePubSub(AwsExports.config);

open Types;

[@react.component]
let make = () => {
  let (message, setMessage) = React.useState(() => None);
  let (value, setValue) = React.useState(() => "");

  let handleSubmit = event => {
    // Demo.testQuery() |> ignore;
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
      variables: Some(mutationRequest##variables),
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

  React.useEffect(() => {
    let eventJs = event => {
      Js.log2("in EventJS", event);
      {j|{
     			next: (event) => {
     				if (event) {
     					console.log('Subscription_REASON: ' + JSON.stringify(event.value.data, null, 2));
     					console.log('EVENT_REASON: ' + JSON.stringify(event, null, 2));
     					setDisplay(true);
     					let message = event.value.data.onCreateMessage.message;
     					setMessage(message);
     				}
     			}
     		}|j};
    };

    let subRequest = Graphql.OnCreateMessage.make();
    let graphqlOperation: Types.graphqlOperation = {
      query: subRequest##query,
      variables: Some(subRequest##variables),
    };
    Js.log2("IN SUB USEFFECT: graphqlOperation", graphqlOperation);
    // let sub = API.subWithWonka(graphqlOperation);
    // let graphqlSubCb = API.graphqlSubCb(graphqlOperation);

    // let _ =
    //   graphqlSubCb
    //   |> Wonka.fromObservable
    //   |> Wonka.subscribe((. event) => {
    //        Js.log2("data", event);
    //        ();
    //      });
    let request = Types.OnCreateMessage.make();
    // let executeSubscription = () => API.executeSubscription(~request);
    // executeSubscription()
    // |> Wonka.subscribe((. {Types.response}) =>
    //      switch (response) {
    //      | Data(data) =>
    //        let stateHash = data;
    //        Js.log2("Already processed block: %s", stateHash);
    //      // if (BlockSet.has(processedBlocks, stateHash)) {
    //      //   Js.log2("Already processed block: %s", stateHash);
    //      //   false;
    //      // } else {
    //      //   BlockSet.add(processedBlocks, stateHash);
    //      //   true;
    //      // };
    //      | Error(_)
    //      | NotFound => Js.log("NotFound")
    //      }
    //    )
    // |> ignore;
    //  {
    //      Js.log2("executeSubscription_RESPONSE: ", jsonStringify(r, Js.Nullable.null, 2))
    //    })|>ignore;

    let graphqlSubUrql = API.graphqlSubUrql(graphqlOperation);
    let _ =
      graphqlSubUrql
      |> Wonka.fromObservable
      |> Wonka.subscribe((. {Types.response}) => {
        switch (response) {
        | Data(data) =>
          let stateHash = data;
          Js.log2("Already processed block: %s", stateHash);
        // if (BlockSet.has(processedBlocks, stateHash)) {
        //   Js.log2("Already processed block: %s", stateHash);
        //   false;
        // } else {
        //   BlockSet.add(processedBlocks, stateHash);
        //   true;
        // };
        | Error(_)
        | NotFound => Js.log("NotFound")
        };
          //  Js.log2(
          //    "testQuery",
          //    data,
             //  switch (data.response) {
             //  | Data(d) =>
             //    switch (Js.Json.stringifyAny(d)) {
             //    | Some(s) => Js.log2("testQuery", s)
             //    | None => ()
             //    }
             //  | Error(e) =>
             //    switch (Js.Json.stringifyAny(e)) {
             //    | Some(s) => Js.log2("testQuery", s)
             //    | None => ()
             //    }
             //  | _ => ()
             //  }
          //  )
         });
    // |> Wonka.subscribe((. event) => {
    //      Js.log2("graphqlSubUrql_data", event);
    //      ();
    //    });

    // let _ =
    //   sub
    //   |> Wonka.fromObservable
    //   |> Wonka.subscribe((. x) => Js.log2("obs", x));

    None;
  });
  let handleChange = e => {
    let value = e |> getInputValue;
    setValue(_ => value);
  };
  <div className="container">
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
    {switch (message) {
     | Some(message) =>
       <div className="container">
         <div className="card bg-success">
           <h3 className="card-text text-white p-2">
             message->React.string
           </h3>
         </div>
       </div>
     | None => React.null
     }}
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
};
let default = make;