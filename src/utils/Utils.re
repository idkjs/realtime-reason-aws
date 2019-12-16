[@bs.val]
external jsonStringify: ('a, Js.Nullable.t(unit), int) => string =
  "JSON.stringify";

let getInputValue = (e): string => ReactEvent.Form.target(e)##value;

let extractMessageFrom = event => {
  let event = event->Obj.magic;
  /* get the message value on event and post to ui */
  let message = event##value##data##onCreateMessage##message;
  message;
};