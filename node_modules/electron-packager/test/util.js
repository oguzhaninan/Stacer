'use strict'

const bufferEqual = require('buffer-equal')
const common = require('../common')
const config = require('./config.json')
const fs = require('fs-extra')
const path = require('path')
const series = require('run-series')
const slice = Array.prototype.slice
const test = require('tape')

const ORIGINAL_CWD = process.cwd()
const WORK_CWD = path.join(__dirname, 'work')

exports.areFilesEqual = function areFilesEqual (file1, file2, callback) {
  series([
    function (cb) {
      fs.readFile(file1, cb)
    },
    function (cb) {
      fs.readFile(file2, cb)
    }
  ], function (err, buffers) {
    callback(err, bufferEqual(buffers[0], buffers[1]))
  })
}

exports.downloadAll = function downloadAll (version, callback) {
  let combinations = common.createDownloadCombos({electronVersion: config.version}, common.platforms, common.archs, (platform, arch) => {
    // Skip testing darwin/mas target on Windows since electron-packager itself skips it
    // (see https://github.com/electron-userland/electron-packager/issues/71)
    return common.isPlatformMac(platform) && process.platform === 'win32'
  })

  series(combinations.map(combination => {
    return (cb) => {
      var downloadOpts = Object.assign({}, combination)
      downloadOpts.version = version
      downloadOpts.quiet = !!process.env.CI
      common.downloadElectronZip(downloadOpts, cb)
    }
  }), callback)
}

exports.fixtureSubdir = function fixtureSubdir (subdir) {
  return path.join(__dirname, 'fixtures', subdir)
}

exports.generateResourcesPath = function generateResourcesPath (opts) {
  return common.isPlatformMac(opts.platform)
    ? path.join(opts.name + '.app', 'Contents', 'Resources')
    : 'resources'
}

exports.getWorkCwd = function getWorkCwd () {
  return WORK_CWD
}

// tape doesn't seem to have a provision for before/beforeEach/afterEach/after,
// so run setup/teardown and cleanup tasks as additional "tests" to put them in sequence
// and run them irrespective of test failures

exports.setup = function setup () {
  test('setup', function (t) {
    fs.mkdirp(WORK_CWD, function (err) {
      if (err) t.end(err)
      process.chdir(WORK_CWD)
      t.end()
    })
  })
}

exports.teardown = function teardown () {
  test('teardown', function (t) {
    process.chdir(ORIGINAL_CWD)
    fs.remove(WORK_CWD, function (err) {
      t.end(err)
    })
  })
}

exports.packagerTest = function packagerTest (name, testFunction) {
  exports.setup()
  test(name, testFunction)
  exports.teardown()
}

exports.testSinglePlatform = function testSinglePlatform (name, createTest /*, ...createTestArgs */) {
  var args = slice.call(arguments, 2)
  exports.packagerTest(name, createTest.apply(null, [{platform: 'linux', arch: 'x64', electronVersion: config.version}].concat(args)))
}

exports.verifyPackageExistence = function verifyPackageExistence (finalPaths, callback) {
  series(finalPaths.map((finalPath) => {
    return (cb) => {
      fs.stat(finalPath, cb)
    }
  }), (err, statsResults) => {
    if (err) return callback(null, false)

    callback(null, statsResults.every((stats) => {
      return stats.isDirectory()
    }))
  })
}
