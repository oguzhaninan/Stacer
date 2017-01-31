(function() {
  var createSnapshot, dumpObjectToJS, fs, mksnapshot, path, stripBOM, vm, wrapModuleCode;

  fs = require('fs');

  path = require('path');

  mksnapshot = require('mksnapshot');

  vm = require('vm');

  stripBOM = function(content) {
    if (content.charCodeAt(0) === 0xFEFF) {
      content = content.slice(1);
    }
    return content;
  };

  wrapModuleCode = function(script) {
    script = script.replace(/^\#\!.*/, '');
    return "(function(exports, require, module, __filename, __dirname) { " + script + " \n});";
  };

  dumpObjectToJS = function(content) {
    var filename, func, result;
    result = 'var __ATOM_SHELL_SNAPSHOT = {\n';
    for (filename in content) {
      func = content[filename].toString();
      result += "  '" + filename + "': " + func + ",\n";
    }
    result += '};\n';
    return result;
  };

  createSnapshot = function(src, dest, filenames, metadata, options, callback) {
    var arch, builddir, compiled, content, error, file, filename, i, len, relativeFilename, script, snapshotdir, str, target, version;
    try {
      src = path.resolve(src);
      content = {};
      for (i = 0, len = filenames.length; i < len; i++) {
        filename = filenames[i];
        file = metadata[filename];
        if ((file.type === 'file' || file.type === 'link') && filename.substr(-3) === '.js') {
          script = wrapModuleCode(stripBOM(fs.readFileSync(filename, 'utf8')));
          relativeFilename = path.relative(src, filename);
          try {
            compiled = vm.runInThisContext(script, {
              filename: relativeFilename
            });
            content[relativeFilename] = compiled;
          } catch (_error) {
            error = _error;
            console.error('Ignoring ' + relativeFilename + ' for ' + error.name);
          }
        }
      }
    } catch (_error) {
      error = _error;
      return callback(error);
    }
    str = dumpObjectToJS(content);
    version = options.version, arch = options.arch, builddir = options.builddir, snapshotdir = options.snapshotdir;
    if (snapshotdir == null) {
      snapshotdir = path.dirname(dest);
    }
    target = path.resolve(snapshotdir, 'snapshot_blob.bin');
    return mksnapshot(str, target, version, arch, builddir, callback);
  };

  module.exports = createSnapshot;

}).call(this);
