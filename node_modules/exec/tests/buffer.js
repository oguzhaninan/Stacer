var assert = require('assert');

var exec = require('..');

var opts = {
  encoding: 'buffer'
};

exec(['ls', '-lh', '-a'], opts, function(err, out, code) {
  if (err instanceof Error)
    throw err;
  assert(Buffer.isBuffer(err));
  assert(Buffer.isBuffer(out));
  process.exit(code);
});
