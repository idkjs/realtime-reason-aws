// Generated by BUCKLESCRIPT, PLEASE EDIT WITH CARE
'use strict';


function getInputValue(e) {
  return e.target.value;
}

function extractMessageFrom($$event) {
  return $$event.value.data.onCreateMessage.message;
}

exports.getInputValue = getInputValue;
exports.extractMessageFrom = extractMessageFrom;
/* No side effect */
