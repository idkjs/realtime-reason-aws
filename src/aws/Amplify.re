type t;
[@bs.module "@aws-amplify/core"] external amplify: t = "default";
[@bs.send] external configure: (t, AwsExports.t) => unit = "configure";
let configure = config => configure(amplify, config);