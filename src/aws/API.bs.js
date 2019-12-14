// Generated by BUCKLESCRIPT, PLEASE EDIT WITH CARE
'use strict';

var Wonka = require("wonka/src/wonka.js");
var Api = require("@aws-amplify/api");

function configure(config) {
  Api.default.configure(config);
  return /* () */0;
}

function listener_next($$event) {
  console.log("Subscription: ", JSON.stringify($$event.value.data, null, 2));
  console.log("EVENT: ", JSON.stringify($$event, null, 2));
  var message = $$event.value.data.message;
  console.log("MESSAGE: ", JSON.stringify(message, null, 2));
  return /* () */0;
}

function listener_error(errorValue) {
  console.log(errorValue);
  return /* () */0;
}

function listener_complete(param) {
  console.log("COMPLETE");
  return /* () */0;
}

var listener = {
  next: listener_next,
  error: listener_error,
  complete: listener_complete
};

function mutate(graphqlOperation) {
  return Api.default.graphql(graphqlOperation);
}

function subscribe(graphqlOperation) {
  return Api.default.graphql(graphqlOperation);
}

function extractMessageFrom($$event) {
  return $$event.value.data.onCreateMessage.message;
}

function subscribeToMessage(graphqlOperation) {
  return Wonka.map(extractMessageFrom)(Wonka.fromObservable(Api.default.graphql(graphqlOperation)));
}

exports.configure = configure;
exports.listener = listener;
exports.mutate = mutate;
exports.subscribe = subscribe;
exports.extractMessageFrom = extractMessageFrom;
exports.subscribeToMessage = subscribeToMessage;
/* Wonka Not a pure module */
