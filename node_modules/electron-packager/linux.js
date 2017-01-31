'use strict'

const common = require('./common')
const path = require('path')

module.exports = {
  createApp: function createApp (opts, templatePath, callback) {
    common.initializeApp(opts, templatePath, path.join('resources', 'app'), function buildLinuxApp (err, tempPath) {
      if (err) return callback(err)
      common.rename(tempPath, 'electron', common.sanitizeAppName(opts.name), function (err) {
        if (err) return callback(err)
        common.moveApp(opts, tempPath, callback)
      })
    })
  }
}
