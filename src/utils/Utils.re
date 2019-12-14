[@bs.val]
external jsonStringify: ('a, Js.Nullable.t(unit), int) => string =
  "JSON.stringify";

let getInputValue = (e): string => ReactEvent.Form.target(e)##value;