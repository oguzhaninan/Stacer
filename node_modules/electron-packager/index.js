'use strict'

const common = require('./common')
const debug = require('debug')('electron-packager')
const extract = require('extract-zip')
const fs = require('fs-extra')
const getPackageInfo = require('get-package-info')
const ignore = require('./ignore')
const metadata = require('./package.json')
const os = require('os')
const path = require('path')
const resolve = require('resolve')
const series = require('run-series')
const targets = require('./targets')

function debugHostInfo () {
  debug(`Electron Packager ${metadata.version}`)
  debug(`Node ${process.version}`)
  debug(`Host Operating system: ${process.platform} (${process.arch})`)
}

function getMetadata (opts, dir, cb) {
  var props = []
  if (!opts.name) props.push(['productName', 'name'])
  if (!opts['app-version']) props.push('version')
  if (!opts.electronVersion) {
    props.push([
      'dependencies.electron',
      'devDependencies.electron',
      'dependencies.electron-prebuilt',
      'devDependencies.electron-prebuilt'
    ])
  }

  // Name and version provided, no need to infer
  if (props.length === 0) return cb(null)

  // Search package.json files to infer name and version from
  getPackageInfo(props, dir, (err, result) => {
    if (err && err.missingProps) {
      let missingProps = err.missingProps.map(prop => {
        return Array.isArray(prop) ? prop[0] : prop
      })

      if (isMissingRequiredProperty(missingProps)) {
        let messages = missingProps.map(errorMessageForProperty)

        debug(err.message)
        err.message = messages.join('\n') + '\n'
        return cb(err)
      } else {
        // Missing props not required, can continue w/ partial result
        result = err.result
      }
    } else if (err) {
      return cb(err)
    }

    if (result.values.productName) {
      debug(`Inferring application name from ${result.source.productName.prop} in ${result.source.productName.src}`)
      opts.name = result.values.productName
    }

    if (result.values.version) {
      debug(`Inferring app-version from version in ${result.source.version.src}`)
      opts['app-version'] = result.values.version
    }

    if (result.values['dependencies.electron']) {
      let prop = result.source['dependencies.electron'].prop.split('.')[1]
      let src = result.source['dependencies.electron'].src
      return getVersion(opts, prop, src, cb)
    } else {
      return cb(null)
    }
  })
}

function isMissingRequiredProperty (props) {
  var requiredProps = props.filter(
    (prop) => prop === 'productName' || prop === 'dependencies.electron'
  )
  return requiredProps.length !== 0
}

function errorMessageForProperty (prop) {
  let hash, propName
  if (prop === 'productName') {
    hash = 'name'
    propName = 'application name'
  }

  if (prop === 'dependencies.electron') {
    hash = 'version'
    propName = 'Electron version'
  }

  return `Unable to determine ${propName}. Please specify an ${propName}\n\n` +
    'For more information, please see\n' +
    `https://github.com/electron-userland/electron-packager/blob/master/docs/api.md#${hash}\n`
}

function getVersion (opts, packageName, src, cb) {
  resolve(packageName, {
    basedir: path.dirname(src)
  }, (err, res, pkg) => {
    if (err) return cb(err)
    debug(`Inferring target Electron version from ${packageName} in ${src}`)
    opts.electronVersion = pkg.version
    return cb(null)
  })
}

function createSeries (opts, archs, platforms) {
  var tempBase = path.join(opts.tmpdir || os.tmpdir(), 'electron-packager')

  function testSymlink (cb) {
    var testPath = path.join(tempBase, 'symlink-test')
    var testFile = path.join(testPath, 'test')
    var testLink = path.join(testPath, 'testlink')
    series([
      function (cb) {
        fs.outputFile(testFile, '', cb)
      },
      function (cb) {
        fs.symlink(testFile, testLink, cb)
      }
    ], function (err) {
      var result = !err
      fs.remove(testPath, function () {
        cb(result) // ignore errors on cleanup
      })
    })
  }

  var tasks = []
  var useTempDir = opts.tmpdir !== false
  if (useTempDir) {
    tasks.push(function (cb) {
      fs.remove(tempBase, cb)
    })
  }
  return tasks.concat(common.createDownloadCombos(opts, platforms, archs).map(combination => {
    var arch = combination.arch
    var platform = combination.platform
    var version = combination.version

    return (callback) => {
      common.downloadElectronZip(combination, (err, zipPath) => {
        if (err) return callback(err)

        function createApp (comboOpts) {
          var buildParentDir
          if (useTempDir) {
            buildParentDir = tempBase
          } else {
            buildParentDir = opts.out || process.cwd()
          }
          var buildDir = path.join(buildParentDir, `${platform}-${arch}-template`)
          common.info(`Packaging app for platform ${platform} ${arch} using electron v${version}`, opts.quiet)
          series([
            function (cb) {
              debug(`Creating ${buildDir}`)
              fs.mkdirs(buildDir, cb)
            },
            function (cb) {
              debug(`Extracting ${zipPath} to ${buildDir}`)
              extract(zipPath, {dir: buildDir}, cb)
            },
            function (cb) {
              if (!opts.afterExtract || !Array.isArray(opts.afterExtract)) {
                cb()
              } else {
                var newFunctions = opts.afterExtract.map(function (fn) {
                  return fn.bind(this, buildDir, version, platform, arch)
                })
                series(newFunctions, cb)
              }
            }
          ], function () {
            require(targets.supportedPlatforms[platform]).createApp(comboOpts, buildDir, callback)
          })
        }

        // Create delegated options object with specific platform and arch, for output directory naming
        var comboOpts = Object.create(opts)
        comboOpts.arch = arch
        comboOpts.platform = platform
        comboOpts.version = version
        comboOpts.afterCopy = opts.afterCopy

        if (!useTempDir) {
          createApp(comboOpts)
          return
        }

        function checkOverwrite () {
          var finalPath = common.generateFinalPath(comboOpts)
          fs.exists(finalPath, function (exists) {
            if (exists) {
              if (opts.overwrite) {
                fs.remove(finalPath, function () {
                  createApp(comboOpts)
                })
              } else {
                common.info(`Skipping ${platform} ${arch} (output dir already exists, use --overwrite to force)`, opts.quiet)
                callback()
              }
            } else {
              createApp(comboOpts)
            }
          })
        }

        if (common.isPlatformMac(combination.platform)) {
          testSymlink(function (result) {
            if (result) return checkOverwrite()

            common.info(`Cannot create symlinks (on Windows hosts, it requires admin privileges); skipping ${combination.platform} platform`, opts.quiet)
            callback()
          })
        } else {
          checkOverwrite()
        }
      })
    }
  }))
}

module.exports = function packager (opts, cb) {
  debugHostInfo()
  if (debug.enabled) debug(`Packager Options: ${JSON.stringify(opts)}`)

  let archs = targets.validateListFromOptions(opts, targets.supportedArchs, 'arch')
  let platforms = targets.validateListFromOptions(opts, targets.supportedPlatforms, 'platform')
  if (!Array.isArray(archs)) return cb(archs)
  if (!Array.isArray(platforms)) return cb(platforms)

  debug(`Target Platforms: ${platforms.join(', ')}`)
  debug(`Target Architectures: ${archs.join(', ')}`)

  common.deprecatedParameter(opts, 'version', 'electronVersion')

  getMetadata(opts, path.resolve(process.cwd(), opts.dir) || process.cwd(), function (err) {
    if (err) return cb(err)

    debug(`Application name: ${opts.name}`)
    debug(`Target Electron version: ${opts.electronVersion}`)

    ignore.generateIgnores(opts)

    series(createSeries(opts, archs, platforms), function (err, appPaths) {
      if (err) return cb(err)

      cb(null, appPaths.filter(function (appPath) {
        // Remove falsy entries (e.g. skipped platforms)
        return appPath
      }))
    })
  })
}
