#!/usr/bin/env node

'use strict'

var nodeVersionInfo = process.versions.node.split('.').map(function (n) { return Number(n) })
if (nodeVersionInfo < [4, 0, 0]) {
  console.error('CANNOT RUN WITH NODE ' + process.versions.node)
  console.error('Electron Packager requires Node 4.0 or above.')
  process.exit(1)
}

// Not consts so that this file can load in Node < 4.0
var common = require('./common')
var fs = require('fs')
var packager = require('./')
var path = require('path')
var usage = fs.readFileSync(path.join(__dirname, 'usage.txt')).toString()

var args = common.parseCLIArgs(process.argv.slice(2))

if (!args.dir) {
  // temporary fix for https://github.com/nodejs/node/issues/6456
  if (process.stderr._handle && process.stderr._handle.setBlocking) {
    process.stderr._handle.setBlocking(true)
  }
  console.error(usage)
  process.exit(1)
}

packager(args, function done (err, appPaths) {
  if (err) {
    if (err.message) console.error(err.message)
    else console.error(err, err.stack)
    process.exit(1)
  }

  if (appPaths.length > 1) console.error('Wrote new apps to:\n' + appPaths.join('\n'))
  else if (appPaths.length === 1) console.error('Wrote new app to', appPaths[0])
})
