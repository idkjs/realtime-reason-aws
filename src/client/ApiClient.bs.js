// Generated by BUCKLESCRIPT, PLEASE EDIT WITH CARE
'use strict';

var Block = require("bs-platform/lib/js/block.js");
var Wonka = require("wonka/src/wonka.js");
var Belt_Option = require("bs-platform/lib/js/belt_Option.js");
var Caml_option = require("bs-platform/lib/js/caml_option.js");
var Api = require("@aws-amplify/api");

var OperationTypeHigherRuntimeCost = {
  subscription: "subscription",
  query: "query",
  mutation: "mutation"
};

var OperationType = { };

var ClientTypes = {
  OperationTypeHigherRuntimeCost: OperationTypeHigherRuntimeCost,
  OperationType: OperationType
};

function make(config) {
  Api.default.configure(config);
  return Api.default;
}

function clientResponseToReason(parse, result) {
  var data = Belt_Option.map(Caml_option.nullable_to_opt(result.data), parse);
  var error = result.error;
  var response = data !== undefined ? /* Data */Block.__(0, [Caml_option.valFromOption(data)]) : (
      (error == null) ? /* NotFound */0 : /* Error */Block.__(1, [error])
    );
  return {
          data: data,
          error: (error == null) ? undefined : Caml_option.some(error),
          response: response
        };
}

function executeQuery(client, request, param) {
  var req = {
    query: request.query,
    variables: request.variables
  };
  var parse = request.parse;
  return Wonka.map((function (result) {
                  return clientResponseToReason(parse, result);
                }))(client.graphql(req));
}

function executeSubscription(client, request, param) {
  var req = {
    query: request.query,
    variables: request.variables
  };
  var parse = request.parse;
  return Wonka.map((function (result) {
                  return clientResponseToReason(parse, result);
                }))(client.graphql(req));
}

exports.ClientTypes = ClientTypes;
exports.make = make;
exports.clientResponseToReason = clientResponseToReason;
exports.executeQuery = executeQuery;
exports.executeSubscription = executeSubscription;
/* Wonka Not a pure module */