var exec = require('..');

var opts = {
  timeout: 1000
};

exec('echo foo; sleep 30; echo bar', opts, function(err, out, code) {
  if (err instanceof Error)
    throw err;
  process.stderr.write(err);
  process.stdout.write(out);
  process.exit(code);
});
