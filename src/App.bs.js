// Generated by BUCKLESCRIPT, PLEASE EDIT WITH CARE
'use strict';

var Curry = require("bs-platform/lib/js/curry.js");
var Wonka = require("wonka/src/wonka.js");
var React = require("react");
var Caml_option = require("bs-platform/lib/js/caml_option.js");
var API$ReactTemplate = require("./aws/API.bs.js");
var Types$ReactTemplate = require("./aws/Types.bs.js");
var Amplify$ReactTemplate = require("./aws/Amplify.bs.js");
var Graphql$ReactTemplate = require("./graphql/Graphql.bs.js");
var AwsExports$ReactTemplate = require("./aws/AwsExports.bs.js");

function getInputValue(e) {
  return e.target.value;
}

((require('./App.css')));

((require('bootstrap/dist/css/bootstrap.min.css')));

((require("@aws-amplify/pubsub")));

Amplify$ReactTemplate.configure(AwsExports$ReactTemplate.config);

API$ReactTemplate.configure(AwsExports$ReactTemplate.config);

function App(Props) {
  var match = React.useState((function () {
          return ;
        }));
  var message = match[0];
  var match$1 = React.useState((function () {
          return "";
        }));
  var setValue = match$1[1];
  var handleSubmit = function ($$event) {
    var time = Date.now();
    var value = "RE: " + new Date(time).toLocaleString();
    Curry._1(setValue, (function (param) {
            return value;
          }));
    $$event.preventDefault();
    $$event.stopPropagation();
    var message = {
      id: undefined,
      message: value,
      createdAt: undefined
    };
    console.log("Message: ", message);
    var mutationRequest = Graphql$ReactTemplate.CreateMessage.make(message, /* () */0);
    var graphqlOperation_query = mutationRequest.query;
    var graphqlOperation_variables = Caml_option.some(mutationRequest.variables);
    var graphqlOperation = {
      query: graphqlOperation_query,
      variables: graphqlOperation_variables
    };
    return API$ReactTemplate.mutate(graphqlOperation).then((function (response) {
                  return Promise.resolve((console.log("reason_broadcaster_mutation", response.data), /* () */0));
                }));
  };
  React.useEffect((function () {
          var subRequest = Graphql$ReactTemplate.OnCreateMessage.make(/* () */0);
          var graphqlOperation_query = subRequest.query;
          var graphqlOperation_variables = Caml_option.some(subRequest.variables);
          var graphqlOperation = {
            query: graphqlOperation_query,
            variables: graphqlOperation_variables
          };
          console.log("IN SUB USEFFECT: graphqlOperation", graphqlOperation);
          Types$ReactTemplate.OnCreateMessage.make(/* () */0);
          var graphqlSubUrql = API$ReactTemplate.graphqlSubUrql(graphqlOperation);
          Wonka.subscribe((function (response) {
                    console.log("response", response);
                    return /* () */0;
                  }))(Wonka.fromObservable(graphqlSubUrql));
          return ;
        }));
  return React.createElement("div", {
              className: "container"
            }, React.createElement("div", {
                  className: "jumbotron jumbotron-fluid p-0"
                }, React.createElement("h2", {
                      className: "center"
                    }, "Reason Broadcaster")), React.createElement("br", undefined), React.createElement("form", {
                  onSubmit: (function (e) {
                      handleSubmit(e);
                      return /* () */0;
                    })
                }, React.createElement("div", {
                      className: "form-group"
                    }, React.createElement("input", {
                          className: "form-control form-control-lg",
                          type: "text",
                          value: match$1[0],
                          onChange: (function (e) {
                              var e$1 = e;
                              var value = e$1.target.value;
                              return Curry._1(setValue, (function (param) {
                                            return value;
                                          }));
                            })
                        }), React.createElement("input", {
                          className: "btn btn-primary",
                          id: "button",
                          type: "submit",
                          value: "Submit"
                        }))), React.createElement("br", undefined), message !== undefined ? React.createElement("div", {
                    className: "container"
                  }, React.createElement("div", {
                        className: "card bg-success"
                      }, React.createElement("h3", {
                            className: "card-text text-white p-2"
                          }, message))) : null);
}

var make = App;

var $$default = App;

exports.getInputValue = getInputValue;
exports.make = make;
exports.$$default = $$default;
exports.default = $$default;
exports.__esModule = true;
/*  Not a pure module */
