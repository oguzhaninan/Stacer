'use strict'

const config = require('./config.json')
const exec = require('child_process').exec
const series = require('run-series')
const util = require('./util')

// Download all Electron distributions before running tests to avoid timing out due to network
// speed. Most tests run with the config.json version, but we have some tests using 0.37.4, and an
// electron module specific test using 1.3.1.
function preDownloadElectron () {
  const versions = [
    config.version,
    '0.37.4',
    '1.3.1'
  ]
  return versions.map((version) => {
    return (cb) => {
      console.log(`Calling electron-download for ${version} before running tests...`)
      util.downloadAll(version, cb)
    }
  })
}

function npmInstallforFixtures () {
  const fixtures = [
    'basic',
    'basic-renamed-to-electron',
    'infer-missing-version-only',
    'el-0374'
  ]
  return fixtures.map((fixture) => {
    return (cb) => {
      console.log(`Running npm install in fixtures/${fixture}...`)
      exec('npm install --no-bin-links', {cwd: util.fixtureSubdir(fixture)}, cb)
    }
  })
}

let setupFuncs = preDownloadElectron().concat(npmInstallforFixtures())

series(setupFuncs, (error) => {
  if (error) {
    console.error(error.stack || error)
    return process.exit(1)
  }

  console.log('Running tests...')
  require('./basic')
  require('./asar')
  require('./cli')
  require('./ignore')
  require('./hooks')
  require('./multitarget')
  require('./win32')

  if (process.platform !== 'win32') {
    // Perform additional tests specific to building for OS X
    require('./darwin')
    require('./mas')
  }
})
