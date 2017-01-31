'use strict'

const config = require('./config.json')
const fs = require('fs')
const packager = require('..')
const path = require('path')
const rcinfo = require('rcinfo')
const test = require('tape')
const util = require('./util')
const waterfall = require('run-waterfall')
const win32 = require('../win32')

const baseOpts = {
  name: 'basicTest',
  dir: util.fixtureSubdir('basic'),
  electronVersion: config.version,
  arch: 'x64',
  platform: 'win32'
}

function generateVersionStringTest (metadataProperties, extraOpts, expectedValues, assertionMsgs) {
  return (t) => {
    t.timeoutAfter(process.platform === 'darwin' ? config.macExecTimeout : config.timeout)

    let appExePath
    let opts = Object.assign({}, baseOpts, extraOpts)

    waterfall([
      (cb) => {
        packager(opts, cb)
      }, (paths, cb) => {
        appExePath = path.join(paths[0], opts.name + '.exe')
        fs.stat(appExePath, cb)
      }, (stats, cb) => {
        t.true(stats.isFile(), 'The expected EXE file should exist')
        rcinfo(appExePath, cb)
      }, (info, cb) => {
        metadataProperties = [].concat(metadataProperties)
        expectedValues = [].concat(expectedValues)
        assertionMsgs = [].concat(assertionMsgs)
        metadataProperties.forEach((property, i) => {
          var value = expectedValues[i]
          var msg = assertionMsgs[i]
          t.equal(info[property], value, msg)
        })
        cb()
      }
    ], (err) => {
      t.end(err)
    })
  }
}

function setFileVersionTest (buildVersion) {
  var opts = {
    'build-version': buildVersion
  }

  return generateVersionStringTest(
    ['ProductVersion', 'FileVersion'],
    opts,
    ['4.99.101.0', buildVersion],
    ['Product version should match package.json version',
      'File version should match build version']
  )
}

function setProductVersionTest (appVersion) {
  var opts = {
    'app-version': appVersion
  }

  return generateVersionStringTest(
    ['ProductVersion', 'FileVersion'],
    opts,
    [appVersion, appVersion],
    ['Product version should match app version',
      'File version should match app version']
  )
}

function defaultProductAndFileVersionToPackageVersionTest () {
  return generateVersionStringTest(
    ['ProductVersion', 'FileVersion'],
    {},
    ['4.99.101.0', '4.99.101.0'],
    ['Product version should match package.json version',
      'File version should match package.json version']
  )
}

function setCopyrightTest (appCopyright) {
  var opts = {
    'app-copyright': appCopyright
  }

  return generateVersionStringTest('LegalCopyright', opts, appCopyright, 'Legal copyright should match app copyright')
}

function setCopyrightAndCompanyNameTest (appCopyright, companyName) {
  var opts = {
    'app-copyright': appCopyright,
    'win32metadata': {
      CompanyName: companyName
    }
  }

  return generateVersionStringTest(
    ['LegalCopyright', 'CompanyName'],
    opts,
    [appCopyright, companyName],
    ['Legal copyright should match app copyright',
      'Company name should match win32metadata value']
  )
}

function setCompanyNameTest (companyName, optName) {
  let opts = {}
  opts[optName] = {
    CompanyName: companyName
  }

  return generateVersionStringTest('CompanyName',
                                   opts,
                                   companyName,
                                   `Company name should match ${optName} value`)
}

test('better error message when wine is not found', (t) => {
  let err = Error('spawn wine ENOENT')
  err.code = 'ENOENT'
  err.syscall = 'spawn wine'

  t.equal(err.message, 'spawn wine ENOENT')
  err = win32.updateWineMissingException(err)
  t.notEqual(err.message, 'spawn wine ENOENT')

  t.end()
})

test('error message unchanged when error not about wine', (t) => {
  let errNotEnoent = Error('unchanged')
  errNotEnoent.code = 'ESOMETHINGELSE'
  errNotEnoent.syscall = 'spawn wine'

  t.equal(errNotEnoent.message, 'unchanged')
  errNotEnoent = win32.updateWineMissingException(errNotEnoent)
  t.equal(errNotEnoent.message, 'unchanged')

  let errNotSpawnWine = Error('unchanged')
  errNotSpawnWine.code = 'ENOENT'
  errNotSpawnWine.syscall = 'spawn foo'

  t.equal(errNotSpawnWine.message, 'unchanged')
  errNotSpawnWine = win32.updateWineMissingException(errNotSpawnWine)
  t.equal(errNotSpawnWine.message, 'unchanged')

  t.end()
})

util.packagerTest('win32 executable name is based on sanitized app name', (t) => {
  let opts = Object.assign({}, baseOpts, {name: '@username/package-name'})

  waterfall([
    (cb) => {
      packager(opts, cb)
    }, (paths, cb) => {
      t.equal(1, paths.length, '1 bundle created')
      let appExePath = path.join(paths[0], '@username-package-name.exe')
      fs.stat(appExePath, cb)
    }, (stats, cb) => {
      t.true(stats.isFile(), 'The sanitized EXE filename should exist')
      cb()
    }
  ], (err) => {
    t.end(err)
  })
})

util.packagerTest('win32 build version sets FileVersion test', setFileVersionTest('2.3.4.5'))
util.packagerTest('win32 app version sets ProductVersion test', setProductVersionTest('5.4.3.2'))
util.packagerTest('win32 app/build versions default to package.json version test', defaultProductAndFileVersionToPackageVersionTest())
util.packagerTest('win32 app copyright sets LegalCopyright test', setCopyrightTest('Copyright Bar'))
util.packagerTest('win32 set LegalCopyright and CompanyName test', setCopyrightAndCompanyNameTest('Copyright Bar', 'MyCompany LLC'))
util.packagerTest('win32 set CompanyName test (win32metadata)', setCompanyNameTest('MyCompany LLC', 'win32metadata'))
util.packagerTest('win32 set CompanyName test (version-string)', setCompanyNameTest('MyCompany LLC', 'version-string'))
