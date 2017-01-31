'use strict'

var _ = require('lodash')
var asar = require('asar')
var async = require('async')
var child = require('child_process')
var debug = require('debug')
var fs = require('fs-extra')
var fsize = require('get-folder-size')
var glob = require('glob')
var path = require('path')
var temp = require('temp').track()
var wrap = require('word-wrap')

var pkg = require('../package.json')

var defaultLogger = debug(pkg.name)

var defaultRename = function (dest, src) {
  return path.join(dest, '<%= name %>_<%= version %>_<%= arch %>.deb')
}

/**
 * Spawn a child process.
 */
var spawn = function (options, command, args, callback) {
  var spawnedProcess = null
  var error = null
  var stderr = ''

  options.logger('Executing command ' + command + ' ' + args.join(' '))

  try {
    spawnedProcess = child.spawn(command, args)
  } catch (err) {
    process.nextTick(function () {
      callback(err, stderr)
    })
    return
  }

  spawnedProcess.stderr.on('data', function (data) {
    stderr += data
  })

  spawnedProcess.on('error', function (err) {
    error = error || err
  })

  spawnedProcess.on('close', function (code, signal) {
    if (code !== 0) {
      error = error || signal || code
    }

    callback(error && new Error('Error executing command (' + (error.message || error) + '): ' +
      '\n' + command + ' ' + args.join(' ') + '\n' + stderr))
  })
}

/**
 * Read `package.json` either from `resources.app.asar` (if the app is packaged)
 * or from `resources/app/package.json` (if it is not).
 */
var readMeta = function (options, callback) {
  var withAsar = path.join(options.src, 'resources/app.asar')
  var withoutAsar = path.join(options.src, 'resources/app/package.json')

  try {
    fs.accessSync(withAsar)
    options.logger('Reading package metadata from ' + withAsar)
    callback(null, JSON.parse(asar.extractFile(withAsar, 'package.json')))
    return
  } catch (err) {
  }

  try {
    options.logger('Reading package metadata from ' + withoutAsar)
    callback(null, fs.readJsonSync(withoutAsar))
  } catch (err) {
    callback(new Error('Error reading package metadata: ' + (err.message || err)))
  }
}

/**
 * Read `LICENSE` from the root of the app.
 */
var readLicense = function (options, callback) {
  var licenseSrc = path.join(options.src, 'LICENSE')
  options.logger('Reading license file from ' + licenseSrc)

  fs.readFile(licenseSrc, callback)
}

/**
 * Get the size of the app.
 */
var getSize = function (options, callback) {
  fsize(options.src, callback)
}

/**
 * Get the hash of default options for the installer. Some come from the info
 * read from `package.json`, and some are hardcoded.
 */
var getDefaults = function (data, callback) {
  async.parallel([
    async.apply(readMeta, data),
    async.apply(getSize, {src: data.src})
  ], function (err, results) {
    var pkg = results[0] || {}
    var size = results[1] || 0

    var defaults = {
      name: pkg.name || 'electron',
      productName: pkg.productName || pkg.name,
      genericName: pkg.genericName || pkg.productName || pkg.name,
      description: pkg.description,
      productDescription: pkg.productDescription || pkg.description,
      // Use '~' on pre-releases for proper debain version ordering.
      // See https://www.debian.org/doc/debian-policy/ch-controlfields.html#s-f-Version
      version: (pkg.version || '0.0.0').replace(/(\d)[_\.\-\+]?((RC|rc|pre|dev|beta|alpha)[_\.\-\+]?\d*)$/, '$1~$2'),
      revision: pkg.revision || '1',

      section: 'utils',
      priority: 'optional',
      arch: undefined,
      size: Math.ceil(size / 1024),

      depends: [
        'git',
        'gconf2',
        'gconf-service',
        'gvfs-bin',
        'libc6',
        'libcap2',
        'libgtk2.0-0',
        'libudev0 | libudev1',
        'libgcrypt11 | libgcrypt20',
        'libnotify4',
        'libnss3',
        'libxtst6',
        'python',
        'xdg-utils'
      ],
      recommends: [
        'lsb-release'
      ],
      suggests: [
        'gir1.2-gnomekeyring-1.0',
        'libgnome-keyring0'
      ],
      enhances: [
      ],
      preDepends: [
      ],

      maintainer: pkg.author && (typeof pkg.author === 'string'
        ? pkg.author.replace(/\s+\([^)]+\)/, '')
        : pkg.author.name +
          (pkg.author.email != null ? ' <' + pkg.author.email + '>' : '')
      ),

      homepage: pkg.homepage || (pkg.author && (typeof pkg.author === 'string'
        ? pkg.author.replace(/.*\(([^)]+)\).*/, '$1')
        : pkg.author.url
      )),

      bin: pkg.name || 'electron',
      icon: path.resolve(__dirname, '../resources/icon.png'),

      categories: [
        'GNOME',
        'GTK',
        'Utility'
      ],

      mimeType: [],

      lintianOverrides: []
    }

    callback(err, defaults)
  })
}

/**
 * Get the hash of options for the installer.
 */
var getOptions = function (data, defaults, callback) {
  // Flatten everything for ease of use.
  var options = _.defaults({}, data, data.options, defaults)

  // Wrap the extended description to avoid lintian warning about
  // `extended-description-line-too-long`.
  options.productDescription = wrap(options.productDescription, {width: 80, indent: ' '})

  callback(null, options)
}

/**
 * Fill in a template with the hash of options.
 */
var generateTemplate = function (options, file, callback) {
  options.logger('Generating template from ' + file)

  async.waterfall([
    async.apply(fs.readFile, file),
    function (template, callback) {
      var result = _.template(template)(options)
      options.logger('Generated template from ' + file + '\n' + result)
      callback(null, result)
    }
  ], callback)
}

/**
 * Create the control file for the package.
 *
 * See: https://www.debian.org/doc/debian-policy/ch-controlfields.html
 */
var createControl = function (options, dir, callback) {
  var controlSrc = path.resolve(__dirname, '../resources/control.ejs')
  var controlDest = path.join(dir, 'DEBIAN/control')
  options.logger('Creating control file at ' + controlDest)

  async.waterfall([
    async.apply(generateTemplate, options, controlSrc),
    async.apply(fs.outputFile, controlDest)
  ], function (err) {
    callback(err && new Error('Error creating control file: ' + (err.message || err)))
  })
}

/**
 * Create the binary for the package.
 */
var createBinary = function (options, dir, callback) {
  var binDir = path.join(dir, 'usr/bin')
  var binSrc = path.join('../share', options.name, options.bin)
  var binDest = path.join(binDir, options.name)
  options.logger('Symlinking binary from ' + binSrc + ' to ' + binDest)

  async.waterfall([
    async.apply(fs.ensureDir, binDir),
    async.apply(fs.symlink, binSrc, binDest, 'file')
  ], function (err) {
    callback(err && new Error('Error creating binary file: ' + (err.message || err)))
  })
}

/**
 * Create the desktop file for the package.
 *
 * See: http://standards.freedesktop.org/desktop-entry-spec/latest/
 */
var createDesktop = function (options, dir, callback) {
  var desktopSrc = path.resolve(__dirname, '../resources/desktop.ejs')
  var desktopDest = path.join(dir, 'usr/share/applications', options.name + '.desktop')
  options.logger('Creating desktop file at ' + desktopDest)

  async.waterfall([
    async.apply(generateTemplate, options, desktopSrc),
    async.apply(fs.outputFile, desktopDest)
  ], function (err) {
    callback(err && new Error('Error creating desktop file: ' + (err.message || err)))
  })
}

/**
 * Create pixmap icon for the package.
 */
var createPixmapIcon = function (options, dir, callback) {
  var iconFile = path.join(dir, 'usr/share/pixmaps', options.name + '.png')
  options.logger('Creating icon file at ' + iconFile)

  fs.copy(options.icon, iconFile, function (err) {
    callback(err && new Error('Error creating icon file: ' + (err.message || err)))
  })
}

/**
 * Create hicolor icon for the package.
 */
var createHicolorIcon = function (options, dir, callback) {
  async.forEachOf(options.icon, function (icon, resolution, callback) {
    var iconFile = path.join(dir, 'usr/share/icons/hicolor', resolution, 'apps', options.name + '.png')
    options.logger('Creating icon file at ' + iconFile)

    fs.copy(icon, iconFile, callback)
  }, function (err) {
    callback(err && new Error('Error creating icon file: ' + (err.message || err)))
  })
}

/**
 * Create icon for the package.
 */
var createIcon = function (options, dir, callback) {
  if (_.isObject(options.icon)) {
    createHicolorIcon(options, dir, callback)
  } else {
    createPixmapIcon(options, dir, callback)
  }
}

/**
 * Create copyright for the package.
 */
var createCopyright = function (options, dir, callback) {
  var copyrightFile = path.join(dir, 'usr/share/doc', options.name, 'copyright')
  options.logger('Creating copyright file at ' + copyrightFile)

  async.waterfall([
    async.apply(readLicense, options),
    async.apply(fs.outputFile, copyrightFile)
  ], function (err) {
    callback(err && new Error('Error creating copyright file: ' + (err.message || err)))
  })
}

/**
 * Create lintian overrides for the package.
 */
var createOverrides = function (options, dir, callback) {
  var overridesSrc = path.resolve(__dirname, '../resources/overrides.ejs')
  var overridesDest = path.join(dir, 'usr/share/lintian/overrides', options.name)
  options.logger('Creating lintian overrides at ' + overridesDest)

  async.waterfall([
    async.apply(generateTemplate, options, overridesSrc),
    async.apply(fs.outputFile, overridesDest)
  ], function (err) {
    callback(err && new Error('Error creating lintian overrides file: ' + (err.message || err)))
  })
}

/**
 * Copy the application into the package.
 */
var createApplication = function (options, dir, callback) {
  var applicationDir = path.join(dir, 'usr/share', options.name)
  options.logger('Copying application to ' + applicationDir)

  async.waterfall([
    async.apply(fs.ensureDir, applicationDir),
    async.apply(fs.copy, options.src, applicationDir)
  ], function (err) {
    callback(err && new Error('Error copying application directory: ' + (err.message || err)))
  })
}

/**
 * Create temporary directory where the contents of the package will live.
 */
var createDir = function (options, callback) {
  options.logger('Creating temporary directory')

  async.waterfall([
    async.apply(temp.mkdir, 'electron-'),
    function (dir, callback) {
      dir = path.join(dir, options.name + '_' + options.version + '_' + options.arch)
      fs.ensureDir(dir, callback)
    }
  ], function (err, dir) {
    callback(err && new Error('Error creating temporary directory: ' + (err.message || err)), dir)
  })
}

/**
 * Create the contents of the package.
 */
var createContents = function (options, dir, callback) {
  options.logger('Creating contents of package')

  async.parallel([
    async.apply(createControl, options, dir),
    async.apply(createBinary, options, dir),
    async.apply(createDesktop, options, dir),
    async.apply(createIcon, options, dir),
    async.apply(createCopyright, options, dir),
    async.apply(createOverrides, options, dir),
    async.apply(createApplication, options, dir)
  ], function (err) {
    callback(err, dir)
  })
}

/**
 * Package everything using `dpkg` and `fakeroot`.
 */
var createPackage = function (options, dir, callback) {
  options.logger('Creating package at ' + dir)

  spawn(options, 'fakeroot', ['dpkg-deb', '--build', dir], function (err) {
    callback(err, dir)
  })
}

/**
 * Move the package to the specified destination.
 */
var movePackage = function (options, dir, callback) {
  options.logger('Moving package to destination')

  var packagePattern = path.join(dir, '../*.deb')
  async.waterfall([
    async.apply(glob, packagePattern),
    function (files, callback) {
      async.each(files, function (file) {
        var dest = options.rename(options.dest, path.basename(file))
        dest = _.template(dest)(options)
        options.logger('Moving file ' + file + ' to ' + dest)
        fs.move(file, dest, {clobber: true}, callback)
      }, callback)
    }
  ], function (err) {
    callback(err && new Error('Error moving package files: ' + (err.message || err)), dir)
  })
}

/* ************************************************************************** */

module.exports = function (data, callback) {
  data.rename = data.rename || defaultRename
  data.logger = data.logger || defaultLogger

  async.waterfall([
    async.apply(getDefaults, data),
    async.apply(getOptions, data),
    function (options, callback) {
      data.logger('Creating package with options\n' + JSON.stringify(options, null, 2))
      async.waterfall([
        async.apply(createDir, options),
        async.apply(createContents, options),
        async.apply(createPackage, options),
        async.apply(movePackage, options)
      ], function (err) {
        callback(err, options)
      })
    }
  ], function (err, options) {
    if (!err) {
      data.logger('Successfully created package at ' + options.dest)
    } else {
      data.logger('Error creating package: ' + (err.message || err))
    }

    callback(err, options)
  })
}
