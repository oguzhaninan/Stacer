'use strict'

const config = require('./config.json')
const packager = require('..')
const util = require('./util')
const waterfall = require('run-waterfall')

function createHookTest (hookName) {
  util.packagerTest('platform=all test (one arch) (' + hookName + ' hook)', (t) => {
    t.timeoutAfter(config.timeout * 2) // 2 packages will be built during this test

    var hookCalled = false
    var opts = {
      name: 'basicTest',
      dir: util.fixtureSubdir('basic'),
      electronVersion: config.version,
      arch: 'ia32',
      platform: 'all'
    }

    opts[hookName] = [function testHook (buildPath, electronVersion, platform, arch, callback) {
      hookCalled = true
      t.equal(electronVersion, opts.electronVersion, hookName + ' electronVersion should be the same as the options object')
      t.equal(arch, opts.arch, hookName + ' arch should be the same as the options object')
      callback()
    }]

    waterfall([
      function (cb) {
        packager(opts, cb)
      }, function (finalPaths, cb) {
        t.equal(finalPaths.length, 2, 'packager call should resolve with expected number of paths')
        t.true(hookCalled, hookName + ' methods should have been called')
        util.verifyPackageExistence(finalPaths, cb)
      }, function (exists, cb) {
        t.true(exists, 'Packages should be generated for both 32-bit platforms')
        cb()
      }
    ], function (err) {
      t.end(err)
    })
  })
}

createHookTest('afterCopy')
createHookTest('afterExtract')
