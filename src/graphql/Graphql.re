/* const createMessage = `mutation createMessage($message: String!){
       createMessage(input:{message:$message}) {
       __typename
       id
       message
       createdAt
       }
   }
   `;

   const onCreateMessage = `subscription onCreateMessage {
       onCreateMessage {
       __typename
       message
       }
   }`; */
module ListMessages = [%graphql
  {|
    query ListMessages(
  $filter: ModelMessageFilterInput
  $limit: Int
  $nextToken: String
) {
  listMessages(filter: $filter, limit: $limit, nextToken: $nextToken) {
    items {
      id
      message
      createdAt
    }
    nextToken
  }
}
|}
];
module CreateMessage = [%graphql
  {|
    mutation CreateMessage($input: CreateMessageInput!) {
  createMessage(input: $input) {
    id
    message
    createdAt
  }
  }
|}
];
module OnCreateMessage = [%graphql
  {|
    subscription onCreateMessage {
      onCreateMessage {
      __typename
      message
      }
    }
|}
];