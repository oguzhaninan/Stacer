var assert = require('assert');

var exec = require('..');

process.env.FOO = 'BAR';

exec('echo "$FOO"', function(err, out, code) {
  if (err instanceof Error)
    throw err;
  assert.strictEqual(out.trim(), process.env.FOO);
  process.stderr.write(err);
  process.stdout.write(out);
  process.exit(code);
});
