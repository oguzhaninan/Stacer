'use strict'

const config = require('./config.json')
const exec = require('child_process').exec
const fs = require('fs')
const mac = require('../mac')
const packager = require('..')
const path = require('path')
const plist = require('plist')
const test = require('tape')
const util = require('./util')
const waterfall = require('run-waterfall')

function getHelperExecutablePath (helperName) {
  return path.join(`${helperName}.app`, 'Contents', 'MacOS', helperName)
}

function createHelperAppPathsTest (baseOpts, expectedName) {
  return (t) => {
    t.timeoutAfter(config.timeout)

    let opts = Object.create(baseOpts)
    let frameworksPath

    if (!expectedName) {
      expectedName = opts.name
    }

    waterfall([
      (cb) => {
        packager(opts, cb)
      }, (paths, cb) => {
        frameworksPath = path.join(paths[0], `${expectedName}.app`, 'Contents', 'Frameworks')
        // main Helper.app is already tested in basic test suite; test its executable and the other helpers
        fs.stat(path.join(frameworksPath, getHelperExecutablePath(`${expectedName} Helper`)), cb)
      }, (stats, cb) => {
        t.true(stats.isFile(), 'The Helper.app executable should reflect sanitized opts.name')
        fs.stat(path.join(frameworksPath, `${expectedName} Helper EH.app`), cb)
      }, (stats, cb) => {
        t.true(stats.isDirectory(), 'The Helper EH.app should reflect sanitized opts.name')
        fs.stat(path.join(frameworksPath, getHelperExecutablePath(`${expectedName} Helper EH`)), cb)
      }, (stats, cb) => {
        t.true(stats.isFile(), 'The Helper EH.app executable should reflect sanitized opts.name')
        fs.stat(path.join(frameworksPath, `${expectedName} Helper NP.app`), cb)
      }, (stats, cb) => {
        t.true(stats.isDirectory(), 'The Helper NP.app should reflect sanitized opts.name')
        fs.stat(path.join(frameworksPath, getHelperExecutablePath(`${expectedName} Helper NP`)), cb)
      }, (stats, cb) => {
        t.true(stats.isFile(), 'The Helper NP.app executable should reflect sanitized opts.name')
        cb()
      }
    ], (err) => {
      t.end(err)
    })
  }
}

function createIconTest (baseOpts, icon, iconPath) {
  return function (t) {
    t.timeoutAfter(config.timeout)

    var opts = Object.create(baseOpts)
    opts.icon = icon

    var resourcesPath
    var plistPath

    waterfall([
      function (cb) {
        packager(opts, cb)
      }, function (paths, cb) {
        resourcesPath = path.join(paths[0], util.generateResourcesPath(opts))
        plistPath = path.join(paths[0], opts.name + '.app', 'Contents', 'Info.plist')
        fs.stat(resourcesPath, cb)
      }, function (stats, cb) {
        t.true(stats.isDirectory(), 'The output directory should contain the expected resources subdirectory')
        fs.stat(plistPath, cb)
      }, function (stats, cb) {
        t.true(stats.isFile(), 'The expected Info.plist file should exist')
        fs.readFile(plistPath, 'utf8', cb)
      }, function (file, cb) {
        var obj = plist.parse(file)
        util.areFilesEqual(iconPath, path.join(resourcesPath, obj.CFBundleIconFile), cb)
      }, function (equal, cb) {
        t.true(equal, 'installed icon file should be identical to the specified icon file')
        cb()
      }
    ], function (err) {
      t.end(err)
    })
  }
}

function createExtraResourceTest (baseOpts) {
  return function (t) {
    t.timeoutAfter(config.timeout)

    var extra1Base = 'data1.txt'
    var extra1Path = path.join(__dirname, 'fixtures', extra1Base)

    var opts = Object.create(baseOpts)
    opts['extra-resource'] = extra1Path

    var resourcesPath

    waterfall([
      function (cb) {
        packager(opts, cb)
      }, function (paths, cb) {
        resourcesPath = path.join(paths[0], util.generateResourcesPath(opts))
        fs.stat(resourcesPath, cb)
      }, function (stats, cb) {
        t.true(stats.isDirectory(), 'The output directory should contain the expected resources subdirectory')
        util.areFilesEqual(extra1Path, path.join(resourcesPath, extra1Base), cb)
      }, function (equal, cb) {
        t.true(equal, 'resource file data1.txt should match')
        cb()
      }
    ], function (err) {
      t.end(err)
    })
  }
}

function createExtraResource2Test (baseOpts) {
  return function (t) {
    t.timeoutAfter(config.timeout)

    var extra1Base = 'data1.txt'
    var extra1Path = path.join(__dirname, 'fixtures', extra1Base)
    var extra2Base = 'extrainfo.plist'
    var extra2Path = path.join(__dirname, 'fixtures', extra2Base)

    var opts = Object.create(baseOpts)
    opts['extra-resource'] = [ extra1Path, extra2Path ]

    var resourcesPath

    waterfall([
      function (cb) {
        packager(opts, cb)
      }, function (paths, cb) {
        resourcesPath = path.join(paths[0], util.generateResourcesPath(opts))
        fs.stat(resourcesPath, cb)
      }, function (stats, cb) {
        t.true(stats.isDirectory(), 'The output directory should contain the expected resources subdirectory')
        util.areFilesEqual(extra1Path, path.join(resourcesPath, extra1Base), cb)
      }, function (equal, cb) {
        t.true(equal, 'resource file data1.txt should match')
        util.areFilesEqual(extra2Path, path.join(resourcesPath, extra2Base), cb)
      }, function (equal, cb) {
        t.true(equal, 'resource file extrainfo.plist should match')
        cb()
      }
    ], function (err) {
      t.end(err)
    })
  }
}

function createExtendInfoTest (baseOpts, extraPathOrParams) {
  return function (t) {
    t.timeoutAfter(config.timeout)

    var opts = Object.create(baseOpts)
    opts['extend-info'] = extraPathOrParams
    opts['build-version'] = '3.2.1'
    opts['app-bundle-id'] = 'com.electron.extratest'
    opts['app-category-type'] = 'public.app-category.music'

    var plistPath

    waterfall([
      function (cb) {
        packager(opts, cb)
      }, function (paths, cb) {
        plistPath = path.join(paths[0], opts.name + '.app', 'Contents', 'Info.plist')
        fs.stat(plistPath, cb)
      }, function (stats, cb) {
        t.true(stats.isFile(), 'The expected Info.plist file should exist')
        fs.readFile(plistPath, 'utf8', cb)
      }, function (file, cb) {
        var obj = plist.parse(file)
        t.equal(obj.TestKeyString, 'String data', 'TestKeyString should come from extend-info')
        t.equal(obj.TestKeyInt, 12345, 'TestKeyInt should come from extend-info')
        t.equal(obj.TestKeyBool, true, 'TestKeyBool should come from extend-info')
        t.deepEqual(obj.TestKeyArray, ['public.content', 'public.data'], 'TestKeyArray should come from extend-info')
        t.deepEqual(obj.TestKeyDict, { Number: 98765, CFBundleVersion: '0.0.0' }, 'TestKeyDict should come from extend-info')
        t.equal(obj.CFBundleVersion, opts['build-version'], 'CFBundleVersion should reflect build-version argument')
        t.equal(obj.CFBundleIdentifier, 'com.electron.extratest', 'CFBundleIdentifier should reflect app-bundle-id argument')
        t.equal(obj.LSApplicationCategoryType, 'public.app-category.music', 'LSApplicationCategoryType should reflect app-category-type argument')
        t.equal(obj.CFBundlePackageType, 'APPL', 'CFBundlePackageType should be Electron default')
        cb()
      }
    ], function (err) {
      t.end(err)
    })
  }
}

function createBinaryNameTest (baseOpts, expectedAppName) {
  return (t) => {
    t.timeoutAfter(config.timeout)

    let opts = Object.create(baseOpts)
    let binaryPath
    let appName = expectedAppName || opts.name

    waterfall([
      (cb) => {
        packager(opts, cb)
      }, (paths, cb) => {
        binaryPath = path.join(paths[0], `${appName}.app`, 'Contents', 'MacOS')
        fs.stat(path.join(binaryPath, appName), cb)
      }, (stats, cb) => {
        t.true(stats.isFile(), 'The binary should reflect a sanitized opts.name')
        cb()
      }
    ], (err) => {
      t.end(err)
    })
  }
}

function createAppVersionTest (baseOpts, appVersion, buildVersion) {
  return function (t) {
    t.timeoutAfter(config.timeout)

    var plistPath
    var opts = Object.create(baseOpts)
    opts['app-version'] = opts['build-version'] = appVersion

    if (buildVersion) {
      opts['build-version'] = buildVersion
    }

    waterfall([
      function (cb) {
        packager(opts, cb)
      }, function (paths, cb) {
        plistPath = path.join(paths[0], opts.name + '.app', 'Contents', 'Info.plist')
        fs.stat(plistPath, cb)
      }, function (stats, cb) {
        t.true(stats.isFile(), 'The expected Info.plist file should exist')
        fs.readFile(plistPath, 'utf8', cb)
      }, function (file, cb) {
        var obj = plist.parse(file)
        t.equal(obj.CFBundleVersion, '' + opts['build-version'], 'CFBundleVersion should reflect build-version')
        t.equal(obj.CFBundleShortVersionString, '' + opts['app-version'], 'CFBundleShortVersionString should reflect app-version')
        t.equal(typeof obj.CFBundleVersion, 'string', 'CFBundleVersion should be a string')
        t.equal(typeof obj.CFBundleShortVersionString, 'string', 'CFBundleShortVersionString should be a string')
        cb()
      }
    ], function (err) {
      t.end(err)
    })
  }
}

function createAppVersionInferenceTest (baseOpts) {
  return function (t) {
    t.timeoutAfter(config.timeout)

    var plistPath

    waterfall([
      function (cb) {
        packager(baseOpts, cb)
      }, function (paths, cb) {
        plistPath = path.join(paths[0], baseOpts.name + '.app', 'Contents', 'Info.plist')
        fs.stat(plistPath, cb)
      }, function (stats, cb) {
        t.true(stats.isFile(), 'The expected Info.plist file should exist')
        fs.readFile(plistPath, 'utf8', cb)
      }, function (file, cb) {
        var obj = plist.parse(file)
        t.equal(obj.CFBundleVersion, '4.99.101', 'CFBundleVersion should reflect package.json version')
        t.equal(obj.CFBundleShortVersionString, '4.99.101', 'CFBundleShortVersionString should reflect package.json version')
        cb()
      }
    ], function (err) {
      t.end(err)
    })
  }
}

function createAppCategoryTypeTest (baseOpts, appCategoryType) {
  return function (t) {
    t.timeoutAfter(config.timeout)

    var plistPath
    var opts = Object.create(baseOpts)
    opts['app-category-type'] = appCategoryType

    waterfall([
      function (cb) {
        packager(opts, cb)
      }, function (paths, cb) {
        plistPath = path.join(paths[0], opts.name + '.app', 'Contents', 'Info.plist')
        fs.stat(plistPath, cb)
      }, function (stats, cb) {
        t.true(stats.isFile(), 'The expected Info.plist file should exist')
        fs.readFile(plistPath, 'utf8', cb)
      }, function (file, cb) {
        var obj = plist.parse(file)
        t.equal(obj.LSApplicationCategoryType, opts['app-category-type'], 'LSApplicationCategoryType should reflect opts["app-category-type"]')
        cb()
      }
    ], function (err) {
      t.end(err)
    })
  }
}

function createAppBundleTest (baseOpts, appBundleId) {
  return function (t) {
    t.timeoutAfter(config.timeout)

    var plistPath
    var opts = Object.create(baseOpts)
    if (appBundleId) {
      opts['app-bundle-id'] = appBundleId
    }
    var defaultBundleName = 'com.electron.' + opts.name.toLowerCase()
    var appBundleIdentifier = mac.filterCFBundleIdentifier(opts['app-bundle-id'] || defaultBundleName)

    waterfall([
      function (cb) {
        packager(opts, cb)
      }, function (paths, cb) {
        plistPath = path.join(paths[0], opts.name + '.app', 'Contents', 'Info.plist')
        fs.stat(plistPath, cb)
      }, function (stats, cb) {
        t.true(stats.isFile(), 'The expected Info.plist file should exist')
        fs.readFile(plistPath, 'utf8', cb)
      }, function (file, cb) {
        var obj = plist.parse(file)
        t.equal(obj.CFBundleDisplayName, opts.name, 'CFBundleDisplayName should reflect opts.name')
        t.equal(obj.CFBundleName, opts.name, 'CFBundleName should reflect opts.name')
        t.equal(obj.CFBundleIdentifier, appBundleIdentifier, 'CFBundleName should reflect opts["app-bundle-id"] or fallback to default')
        t.equal(typeof obj.CFBundleDisplayName, 'string', 'CFBundleDisplayName should be a string')
        t.equal(typeof obj.CFBundleName, 'string', 'CFBundleName should be a string')
        t.equal(typeof obj.CFBundleIdentifier, 'string', 'CFBundleIdentifier should be a string')
        t.equal(/^[a-zA-Z0-9-.]*$/.test(obj.CFBundleIdentifier), true, 'CFBundleIdentifier should allow only alphanumeric (A-Z,a-z,0-9), hyphen (-), and period (.)')
        cb()
      }
    ], function (err) {
      t.end(err)
    })
  }
}

function createAppBundleFrameworkTest (baseOpts) {
  return function (t) {
    t.timeoutAfter(config.timeout)

    var frameworkPath

    waterfall([
      function (cb) {
        packager(baseOpts, cb)
      }, function (paths, cb) {
        frameworkPath = path.join(paths[0], `${baseOpts.name}.app`, 'Contents', 'Frameworks', 'Electron Framework.framework')
        fs.stat(frameworkPath, cb)
      }, function (stats, cb) {
        t.true(stats.isDirectory(), 'Expected Electron Framework.framework to be a directory')
        fs.lstat(path.join(frameworkPath, 'Electron Framework'), cb)
      }, function (stats, cb) {
        t.true(stats.isSymbolicLink(), 'Expected Electron Framework.framework/Electron Framework to be a symlink')
        fs.lstat(path.join(frameworkPath, 'Versions', 'Current'), cb)
      }, function (stats, cb) {
        t.true(stats.isSymbolicLink(), 'Expected Electron Framework.framework/Versions/Current to be a symlink')
        cb()
      }
    ], function (err) {
      t.end(err)
    })
  }
}

function createAppHelpersBundleTest (baseOpts, helperBundleId, appBundleId) {
  return function (t) {
    t.timeoutAfter(config.timeout)

    var tempPath, plistPath
    var opts = Object.create(baseOpts)
    if (helperBundleId) {
      opts['helper-bundle-id'] = helperBundleId
    }
    if (appBundleId) {
      opts['app-bundle-id'] = appBundleId
    }
    var defaultBundleName = 'com.electron.' + opts.name.toLowerCase()
    var appBundleIdentifier = mac.filterCFBundleIdentifier(opts['app-bundle-id'] || defaultBundleName)
    var helperBundleIdentifier = mac.filterCFBundleIdentifier(opts['helper-bundle-id'] || appBundleIdentifier + '.helper')

    waterfall([
      function (cb) {
        packager(opts, cb)
      }, function (paths, cb) {
        tempPath = paths[0]
        plistPath = path.join(tempPath, opts.name + '.app', 'Contents', 'Frameworks', opts.name + ' Helper.app', 'Contents', 'Info.plist')
        fs.stat(plistPath, cb)
      }, function (stats, cb) {
        t.true(stats.isFile(), 'The expected Info.plist file should exist in helper app')
        fs.readFile(plistPath, 'utf8', cb)
      }, function (file, cb) {
        var obj = plist.parse(file)
        t.equal(obj.CFBundleName, opts.name, 'CFBundleName should reflect opts.name in helper app')
        t.equal(obj.CFBundleIdentifier, helperBundleIdentifier, 'CFBundleIdentifier should reflect opts["helper-bundle-id"], opts["app-bundle-id"] or fallback to default in helper app')
        t.equal(typeof obj.CFBundleName, 'string', 'CFBundleName should be a string in helper app')
        t.equal(typeof obj.CFBundleIdentifier, 'string', 'CFBundleIdentifier should be a string in helper app')
        t.equal(/^[a-zA-Z0-9-.]*$/.test(obj.CFBundleIdentifier), true, 'CFBundleIdentifier should allow only alphanumeric (A-Z,a-z,0-9), hyphen (-), and period (.)')
        // check helper EH
        plistPath = path.join(tempPath, opts.name + '.app', 'Contents', 'Frameworks', opts.name + ' Helper EH.app', 'Contents', 'Info.plist')
        fs.stat(plistPath, cb)
      }, function (stats, cb) {
        t.true(stats.isFile(), 'The expected Info.plist file should exist in helper EH app')
        fs.readFile(plistPath, 'utf8', cb)
      }, function (file, cb) {
        var obj = plist.parse(file)
        t.equal(obj.CFBundleName, opts.name + ' Helper EH', 'CFBundleName should reflect opts.name in helper EH app')
        t.equal(obj.CFBundleDisplayName, opts.name + ' Helper EH', 'CFBundleDisplayName should reflect opts.name in helper EH app')
        t.equal(obj.CFBundleExecutable, opts.name + ' Helper EH', 'CFBundleExecutable should reflect opts.name in helper EH app')
        t.equal(obj.CFBundleIdentifier, helperBundleIdentifier + '.EH', 'CFBundleName should reflect opts["helper-bundle-id"], opts["app-bundle-id"] or fallback to default in helper EH app')
        t.equal(typeof obj.CFBundleName, 'string', 'CFBundleName should be a string in helper EH app')
        t.equal(typeof obj.CFBundleDisplayName, 'string', 'CFBundleDisplayName should be a string in helper EH app')
        t.equal(typeof obj.CFBundleExecutable, 'string', 'CFBundleExecutable should be a string in helper EH app')
        t.equal(typeof obj.CFBundleIdentifier, 'string', 'CFBundleIdentifier should be a string in helper EH app')
        t.equal(/^[a-zA-Z0-9-.]*$/.test(obj.CFBundleIdentifier), true, 'CFBundleIdentifier should allow only alphanumeric (A-Z,a-z,0-9), hyphen (-), and period (.)')
        // check helper NP
        plistPath = path.join(tempPath, opts.name + '.app', 'Contents', 'Frameworks', opts.name + ' Helper NP.app', 'Contents', 'Info.plist')
        fs.stat(plistPath, cb)
      }, function (stats, cb) {
        t.true(stats.isFile(), 'The expected Info.plist file should exist in helper NP app')
        fs.readFile(plistPath, 'utf8', cb)
      }, function (file, cb) {
        var obj = plist.parse(file)
        t.equal(obj.CFBundleName, opts.name + ' Helper NP', 'CFBundleName should reflect opts.name in helper NP app')
        t.equal(obj.CFBundleDisplayName, opts.name + ' Helper NP', 'CFBundleDisplayName should reflect opts.name in helper NP app')
        t.equal(obj.CFBundleExecutable, opts.name + ' Helper NP', 'CFBundleExecutable should reflect opts.name in helper NP app')
        t.equal(obj.CFBundleIdentifier, helperBundleIdentifier + '.NP', 'CFBundleName should reflect opts["helper-bundle-id"], opts["app-bundle-id"] or fallback to default in helper NP app')
        t.equal(typeof obj.CFBundleName, 'string', 'CFBundleName should be a string in helper NP app')
        t.equal(typeof obj.CFBundleDisplayName, 'string', 'CFBundleDisplayName should be a string in helper NP app')
        t.equal(typeof obj.CFBundleExecutable, 'string', 'CFBundleExecutable should be a string in helper NP app')
        t.equal(typeof obj.CFBundleIdentifier, 'string', 'CFBundleIdentifier should be a string in helper NP app')
        t.equal(/^[a-zA-Z0-9-.]*$/.test(obj.CFBundleIdentifier), true, 'CFBundleIdentifier should allow only alphanumeric (A-Z,a-z,0-9), hyphen (-), and period (.)')
        cb()
      }
    ], function (err) {
      t.end(err)
    })
  }
}

function createAppHumanReadableCopyrightTest (baseOpts, humanReadableCopyright) {
  return function (t) {
    t.timeoutAfter(config.timeout)

    var plistPath
    var opts = Object.create(baseOpts)
    opts['app-copyright'] = humanReadableCopyright

    waterfall([
      function (cb) {
        packager(opts, cb)
      }, function (paths, cb) {
        plistPath = path.join(paths[0], opts.name + '.app', 'Contents', 'Info.plist')
        fs.stat(plistPath, cb)
      }, function (stats, cb) {
        t.true(stats.isFile(), 'The expected Info.plist file should exist')
        fs.readFile(plistPath, 'utf8', cb)
      }, function (file, cb) {
        var obj = plist.parse(file)
        t.equal(obj.NSHumanReadableCopyright, opts['app-copyright'], 'NSHumanReadableCopyright should reflect opts["app-copyright"]')
        cb()
      }
    ], function (err) {
      t.end(err)
    })
  }
}

function createProtocolTest (baseOpts) {
  return function (t) {
    t.timeoutAfter(config.timeout)

    var plistPath
    var opts = Object.create(baseOpts)
    opts.protocols = [{
      name: 'Foo',
      schemes: ['foo']
    }, {
      name: 'Bar',
      schemes: ['bar', 'baz']
    }]

    waterfall([
      function (cb) {
        packager(opts, cb)
      }, function (paths, cb) {
        plistPath = path.join(paths[0], opts.name + '.app', 'Contents', 'Info.plist')
        fs.stat(plistPath, cb)
      }, function (stats, cb) {
        t.true(stats.isFile(), 'The expected Info.plist file should exist')
        fs.readFile(plistPath, 'utf8', cb)
      }, function (file, cb) {
        t.deepEqual(plist.parse(file).CFBundleURLTypes, [{
          CFBundleURLName: 'Foo',
          CFBundleURLSchemes: ['foo']
        }, {
          CFBundleURLName: 'Bar',
          CFBundleURLSchemes: ['bar', 'baz']
        }], 'CFBundleURLTypes did not contain specified protocol schemes and names')
        cb()
      }
    ], function (err) {
      t.end(err)
    })
  }
}

// Share testing script with platform darwin and mas
module.exports = (baseOpts) => {
  let iconBase = path.join(__dirname, 'fixtures', 'monochrome')
  let icnsPath = iconBase + '.icns'
  let el0374Opts = {
    name: 'el0374Test',
    dir: util.fixtureSubdir('el-0374'),
    electronVersion: '0.37.4',
    arch: 'x64',
    platform: 'darwin'
  }
  let extraInfoPath = path.join(__dirname, 'fixtures', 'extrainfo.plist')
  let extraInfoParams = plist.parse(fs.readFileSync(extraInfoPath).toString())

  util.packagerTest('helper app paths test', createHelperAppPathsTest(baseOpts))
  util.packagerTest('helper app paths test with app name needing sanitization', createHelperAppPathsTest(Object.assign({}, baseOpts, {name: '@username/package-name'}), '@username-package-name'))

  util.packagerTest('icon test: .icns specified', createIconTest(baseOpts, icnsPath, icnsPath))
  util.packagerTest('icon test: el-0.37.4, .icns specified', createIconTest(el0374Opts, icnsPath, icnsPath))
  util.packagerTest('icon test: .ico specified (should replace with .icns)', createIconTest(baseOpts, iconBase + '.ico', icnsPath))
  util.packagerTest('icon test: basename only (should add .icns)', createIconTest(baseOpts, iconBase, icnsPath))

  util.packagerTest('extend-info by filename test', createExtendInfoTest(baseOpts, extraInfoPath))
  util.packagerTest('extend-info by params test', createExtendInfoTest(baseOpts, extraInfoParams))

  util.packagerTest('extra-resource test: one arg', createExtraResourceTest(baseOpts))
  util.packagerTest('extra-resource test: two arg', createExtraResource2Test(baseOpts))

  util.packagerTest('protocol/protocol-name argument test', createProtocolTest(baseOpts))

  test('osx-sign argument test: default args', function (t) {
    var args = true
    var signOpts = mac.createSignOpts(args, 'darwin', 'out', 'version')
    t.same(signOpts, {identity: null, app: 'out', platform: 'darwin', version: 'version'})
    t.end()
  })

  test('osx-sign argument test: identity=true sets autodiscovery mode', function (t) {
    var args = {identity: true}
    var signOpts = mac.createSignOpts(args, 'darwin', 'out', 'version')
    t.same(signOpts, {identity: null, app: 'out', platform: 'darwin', version: 'version'})
    t.end()
  })

  test('osx-sign argument test: entitlements passed to electron-osx-sign', function (t) {
    var args = {entitlements: 'path-to-entitlements'}
    var signOpts = mac.createSignOpts(args, 'darwin', 'out', 'version')
    t.same(signOpts, {app: 'out', platform: 'darwin', version: 'version', entitlements: args.entitlements})
    t.end()
  })

  test('osx-sign argument test: app not overwritten', function (t) {
    var args = {app: 'some-other-path'}
    var signOpts = mac.createSignOpts(args, 'darwin', 'out', 'version')
    t.same(signOpts, {app: 'out', platform: 'darwin', version: 'version'})
    t.end()
  })

  test('osx-sign argument test: platform not overwritten', function (t) {
    var args = {platform: 'mas'}
    var signOpts = mac.createSignOpts(args, 'darwin', 'out', 'version')
    t.same(signOpts, {app: 'out', platform: 'darwin', version: 'version'})
    t.end()
  })

  test('osx-sign argument test: binaries not set', (t) => {
    let args = {binaries: ['binary1', 'binary2']}
    let signOpts = mac.createSignOpts(args, 'darwin', 'out', 'version')
    t.same(signOpts, {app: 'out', platform: 'darwin', version: 'version'})
    t.end()
  })

  util.packagerTest('codesign test', (t) => {
    t.timeoutAfter(config.macExecTimeout)

    var opts = Object.create(baseOpts)
    opts['osx-sign'] = {identity: 'Developer CodeCert'}

    var appPath

    waterfall([
      function (cb) {
        packager(opts, cb)
      }, function (paths, cb) {
        appPath = path.join(paths[0], opts.name + '.app')
        fs.stat(appPath, cb)
      }, function (stats, cb) {
        t.true(stats.isDirectory(), 'The expected .app directory should exist')
        exec('codesign -v ' + appPath, cb)
      }, function (stdout, stderr, cb) {
        t.pass('codesign should verify successfully')
        cb()
      }
    ], function (err) {
      var notFound = err && err.code === 127
      if (notFound) console.log('codesign not installed; skipped')
      t.end(notFound ? null : err)
    })
  })

  util.packagerTest('binary naming test', createBinaryNameTest(baseOpts))
  util.packagerTest('sanitized binary naming test', createBinaryNameTest(Object.assign({}, baseOpts, {name: '@username/package-name'}), '@username-package-name'))

  util.packagerTest('CFBundleName is the sanitized app name and CFBundleDisplayName is the non-sanitized app name', (t) => {
    t.timeoutAfter(config.timeout)

    let plistPath
    let opts = Object.assign({}, baseOpts, {name: '@username/package-name'})
    let appBundleIdentifier = 'com.electron.username-package-name'
    let expectedSanitizedName = '@username-package-name'

    waterfall([
      (cb) => {
        packager(opts, cb)
      }, (paths, cb) => {
        plistPath = path.join(paths[0], `${expectedSanitizedName}.app`, 'Contents', 'Info.plist')
        fs.stat(plistPath, cb)
      }, (stats, cb) => {
        t.true(stats.isFile(), 'The expected Info.plist file should exist')
        fs.readFile(plistPath, 'utf8', cb)
      }, (file, cb) => {
        let obj = plist.parse(file)
        t.equal(typeof obj.CFBundleDisplayName, 'string', 'CFBundleDisplayName should be a string')
        t.equal(obj.CFBundleDisplayName, opts.name, 'CFBundleDisplayName should reflect opts.name')
        t.equal(typeof obj.CFBundleName, 'string', 'CFBundleName should be a string')
        t.equal(obj.CFBundleName, expectedSanitizedName, 'CFBundleName should reflect a sanitized opts.name')
        t.equal(typeof obj.CFBundleIdentifier, 'string', 'CFBundleIdentifier should be a string')
        t.equal(/^[a-zA-Z0-9-.]*$/.test(obj.CFBundleIdentifier), true, 'CFBundleIdentifier should allow only alphanumeric (A-Z,a-z,0-9), hyphen (-), and period (.)')
        t.equal(obj.CFBundleIdentifier, appBundleIdentifier, 'CFBundleIdentifier should reflect the sanitized opts.name')
        cb()
      }
    ], (err) => {
      t.end(err)
    })
  })

  util.packagerTest('app and build version test', createAppVersionTest(baseOpts, '1.1.0', '1.1.0.1234'))
  util.packagerTest('infer app version from package.json test', createAppVersionInferenceTest(baseOpts))
  util.packagerTest('app version test', createAppVersionTest(baseOpts, '1.1.0'))
  util.packagerTest('app and build version integer test', createAppVersionTest(baseOpts, 12, 1234))

  util.packagerTest('app categoryType test', createAppCategoryTypeTest(baseOpts, 'public.app-category.developer-tools'))

  util.packagerTest('app bundle test', createAppBundleTest(baseOpts, 'com.electron.basetest'))
  util.packagerTest('app bundle (w/ special characters) test', createAppBundleTest(baseOpts, 'com.electron."bãśè tëßt!@#$%^&*()?\''))
  util.packagerTest('app bundle app-bundle-id fallback test', createAppBundleTest(baseOpts))
  util.packagerTest('app bundle framework symlink test', createAppBundleFrameworkTest(baseOpts))

  util.packagerTest('app helpers bundle test', createAppHelpersBundleTest(baseOpts, 'com.electron.basetest.helper'))
  util.packagerTest('app helpers bundle (w/ special characters) test', createAppHelpersBundleTest(baseOpts, 'com.electron."bãśè tëßt!@#$%^&*()?\'.hęłpėr'))
  util.packagerTest('app helpers bundle helper-bundle-id fallback to app-bundle-id test', createAppHelpersBundleTest(baseOpts, null, 'com.electron.basetest'))
  util.packagerTest('app helpers bundle helper-bundle-id fallback to app-bundle-id (w/ special characters) test', createAppHelpersBundleTest(baseOpts, null, 'com.electron."bãśè tëßt!!@#$%^&*()?\''))
  util.packagerTest('app helpers bundle helper-bundle-id & app-bundle-id fallback test', createAppHelpersBundleTest(baseOpts))

  util.packagerTest('app humanReadableCopyright test', createAppHumanReadableCopyrightTest(baseOpts, 'Copyright © 2003–2015 Organization. All rights reserved.'))

  util.packagerTest('app named Electron packaged successfully', (t) => {
    let opts = Object.create(baseOpts)
    opts.name = 'Electron'
    let appPath

    waterfall([
      (cb) => {
        packager(opts, cb)
      }, (paths, cb) => {
        appPath = path.join(paths[0], 'Electron.app')
        fs.stat(appPath, cb)
      }, (stats, cb) => {
        t.true(stats.isDirectory(), 'The Electron.app folder exists')
        fs.stat(path.join(appPath, 'Contents', 'MacOS', 'Electron'), cb)
      }, (stats, cb) => {
        t.true(stats.isFile(), 'The Electron.app/Contents/MacOS/Electron binary exists')
        cb()
      }
    ], (err) => {
      t.end(err)
    })
  })
}
