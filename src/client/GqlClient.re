module ApiTypes = ApiTypes;

module Client = {
  type clientResponse('response) =
    ApiClient.ClientTypes.clientResponse('response) = {
      data: option('response),
      error: option(GraphqlError.t),
      response: ApiClient.ClientTypes.response('response),
    };

  include ApiClient;
};

// module Context = ApiContext;
// module Provider = ApiContext.Provider;
// module Consumer = ApiContext.Consumer;

// module Query = ApiQuery;

// module Mutation = ApiMutation;

// module Subscription = ApiSubscription.Subscription;

// module SubscriptionWithHandler = ApiSubscription.SubscriptionWithHandler;

module Request = ApiRequest;

module GraphqlError = GraphqlError;

// module Exchanges = ApiClient.ApiExchanges;

// module Hooks = {
//   type hookResponse('ret) =
//     ApiTypes.hookResponse('ret) = {
//       fetching: bool,
//       data: option('ret),
//       error: option(GraphqlError.t),
//       response: ApiTypes.response('ret),
//     };
//   include ApiUseMutation;
//   include ApiUseQuery;
//   include ApiUseSubscription;
// };