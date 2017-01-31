var spawn = require('child_process').spawn;
var util = require('util');

module.exports = util.deprecate(exec, 'exec: use child_process.execFile instead');

/**
 * Spawn a child with the ease of exec, but safety of spawn
 *
 * @param args      array   ex. [ 'ls', '-lha' ]
 * @param opts      object  [optional] to pass to spawn
 * @param callback  function(err, out, code)
 *
 * @return spawn object
 */
function exec(args, opts, callback) {
  if (arguments.length < 2)
    throw new Error('invalid arguments');
  if (typeof opts === 'function') {
    callback = opts;
    opts = {};
  }
  if (typeof args === 'string') {
    args = ['/bin/sh', '-c', args];
  }
  opts.env = opts.env || process.env;
  opts.encoding = opts.encoding || 'utf-8';
  opts.killSignal = opts.killSignal || 'SIGTERM';

  var out;
  var err;
  var encoding;
  var timeout;
  var done = false;

  // check encoding
  if (opts.encoding !== 'buffer' && Buffer.isEncoding(opts.encoding)) {
    // string
    encoding = opts.encoding;
    out = '';
    err = '';
  } else {
    // buffer
    encoding = null;
    out = [];
    err = [];
  }

  var child = spawn(args[0], args.slice(1), opts);

  if (encoding) {
    if (child.stdout) {
      child.stdout.setEncoding(encoding);
    }
    if (child.stderr) {
      child.stderr.setEncoding(encoding);
    }
  }

  if (child.stdout) {
    child.stdout.on('data', function(data) {
      if (encoding)
        out += data;
      else
        out.push(data);
    });
  }


  if (child.stderr) {
    child.stderr.on('data', function(data) {
      if (encoding)
        err += data;
      else
        err.push(data);
    });
  }

  child.on('close', function(code) {
    if (done)
      return;
    done = true;

    if (timeout)
      clearTimeout(timeout);

    if (!encoding) {
      out = Buffer.concat(out);
      err = Buffer.concat(err);
    }

    callback(err, out, code);
  });

  child.on('error', function(e) {
    if (done)
      return;
    done = true;

    callback(e);
  });

  if (opts.timeout) {
    timeout = setTimeout(function() {
      child.stdout.destroy();
      child.stderr.destroy();
      try {
        child.kill(opts.killSignal);
      } catch(e) {}
      timeout = null;
    }, opts.timeout);
  }

  return child;
}
