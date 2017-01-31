var assert = require('assert');

var exec = require('..');

exec(['this should never exist'], function(err, out, code) {
  assert(err instanceof Error);
  process.exit(0);
});
