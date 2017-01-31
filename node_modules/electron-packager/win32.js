'use strict'

const common = require('./common')
const debug = require('debug')('electron-packager')
const path = require('path')
const series = require('run-series')

function updateWineMissingException (err) {
  if (err && err.code === 'ENOENT' && err.syscall === 'spawn wine') {
    err.message = 'Could not find "wine" on your system.\n\n' +
      'Wine is required to use the app-copyright, app-version, build-version, icon, and \n' +
      'win32metadata parameters for Windows targets.\n\n' +
      'Make sure that the "wine" executable is in your PATH.\n\n' +
      'See https://github.com/electron-userland/electron-packager#building-windows-apps-from-non-windows-platforms for details.'
  }

  return err
}

module.exports = {
  createApp: function createApp (opts, templatePath, callback) {
    common.initializeApp(opts, templatePath, path.join('resources', 'app'), function buildWinApp (err, tempPath) {
      if (err) return callback(err)

      let newExeName = `${common.sanitizeAppName(opts.name)}.exe`
      var operations = [
        function (cb) {
          common.rename(tempPath, 'electron.exe', newExeName, cb)
        }
      ]

      let win32metadata = Object.assign({}, opts['version-string'], opts.win32metadata)

      var rcOpts = {'version-string': win32metadata}

      if (opts['app-version']) {
        rcOpts['product-version'] = rcOpts['file-version'] = opts['app-version']
      }

      if (opts['build-version']) {
        rcOpts['file-version'] = opts['build-version']
      }

      if (opts['app-copyright']) {
        rcOpts['version-string'].LegalCopyright = opts['app-copyright']
      }

      if (opts.icon || opts.win32metadata || opts['version-string'] || opts['app-copyright'] || opts['app-version'] || opts['build-version']) {
        operations.push(function (cb) {
          common.normalizeExt(opts.icon, '.ico', function (err, icon) {
            // Icon might be omitted or only exist in one OS's format, so skip it if normalizeExt reports an error
            if (!err) {
              rcOpts.icon = icon
            }

            debug(`Running rcedit with the options ${JSON.stringify(rcOpts)}`)
            require('rcedit')(path.join(tempPath, newExeName), rcOpts, function (err) {
              cb(updateWineMissingException(err))
            })
          })
        })
      }

      series(operations, function (err) {
        if (err) return callback(err)
        common.moveApp(opts, tempPath, callback)
      })
    })
  },
  updateWineMissingException: updateWineMissingException
}
