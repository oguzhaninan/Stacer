'use strict'

const common = require('../common')
const config = require('./config.json')
const fs = require('fs-extra')
const ignore = require('../ignore')
const path = require('path')
const packager = require('..')
const series = require('run-series')
const test = require('tape')
const util = require('./util')
const waterfall = require('run-waterfall')

function createIgnoreTest (opts, ignorePattern, ignoredFile) {
  return (t) => {
    t.timeoutAfter(config.timeout)

    opts.name = 'basicTest'
    opts.dir = util.fixtureSubdir('basic')
    if (ignorePattern) {
      opts.ignore = ignorePattern
    }

    var appPath

    waterfall([
      (cb) => {
        packager(opts, cb)
      }, (paths, cb) => {
        appPath = path.join(paths[0], util.generateResourcesPath(opts), 'app')
        fs.stat(path.join(appPath, 'package.json'), cb)
      }, (stats, cb) => {
        t.true(stats.isFile(), 'The expected output directory should exist and contain files')
        fs.exists(path.join(appPath, ignoredFile), exists => {
          t.false(exists, 'Ignored file should not exist in output app directory')
          cb()
        })
      }
    ], (err) => {
      t.end(err)
    })
  }
}

function createIgnoreOutDirTest (opts, distPath) {
  return (t) => {
    t.timeoutAfter(config.timeout)

    opts.name = 'basicTest'

    var appDir = util.getWorkCwd()
    opts.dir = appDir
    // we don't use path.join here to avoid normalizing
    var outDir = opts.dir + path.sep + distPath
    opts.out = outDir

    series([
      (cb) => {
        fs.copy(util.fixtureSubdir('basic'), appDir, {
          dereference: true,
          stopOnErr: true,
          filter: file => { return path.basename(file) !== 'node_modules' }
        }, cb)
      },
      (cb) => {
        // create out dir before packager (real world issue - when second run includes unignored out dir)
        fs.mkdirp(outDir, cb)
      },
      (cb) => {
        // create file to ensure that directory will be not ignored because empty
        fs.open(path.join(outDir, 'ignoreMe'), 'w', (err, fd) => {
          if (err) return cb(err)
          fs.close(fd, cb)
        })
      },
      (cb) => {
        packager(opts, cb)
      },
      (cb) => {
        fs.exists(path.join(outDir, common.generateFinalBasename(opts), util.generateResourcesPath(opts), 'app', path.basename(outDir)), (exists) => {
          t.false(exists, 'Out dir must not exist in output app directory')
          cb()
        })
      }
    ], (err) => {
      t.end(err)
    })
  }
}

function createIgnoreImplicitOutDirTest (opts) {
  return (t) => {
    t.timeoutAfter(config.timeout)

    opts.name = 'basicTest'

    var appDir = util.getWorkCwd()
    opts.dir = appDir
    var outDir = opts.dir

    var testFilename = 'ignoreMe'
    var previousPackedResultDir

    series([
      (cb) => {
        fs.copy(util.fixtureSubdir('basic'), appDir, {
          dereference: true,
          stopOnErr: true,
          filter: file => { return path.basename(file) !== 'node_modules' }
        }, cb)
      },
      (cb) => {
        previousPackedResultDir = path.join(outDir, `${common.sanitizeAppName(opts.name)}-linux-ia32`)
        fs.mkdirp(previousPackedResultDir, cb)
      },
      (cb) => {
        // create file to ensure that directory will be not ignored because empty
        fs.open(path.join(previousPackedResultDir, testFilename), 'w', (err, fd) => {
          if (err) return cb(err)
          fs.close(fd, cb)
        })
      },
      (cb) => {
        packager(opts, cb)
      },
      (cb) => {
        fs.exists(path.join(outDir, common.generateFinalBasename(opts), util.generateResourcesPath(opts), 'app', testFilename), (exists) => {
          t.false(exists, 'Out dir must not exist in output app directory')
          cb()
        })
      }
    ], (err) => {
      t.end(err)
    })
  }
}

test('generateOutIgnores ignores all possible platform/arch permutations', (t) => {
  let ignores = ignore.generateOutIgnores({name: 'test'})
  t.equal(ignores.length, common.platforms.length * common.archs.length)
  t.end()
})

util.testSinglePlatform('ignore default test: .o files', createIgnoreTest, null, 'ignore.o')
util.testSinglePlatform('ignore default test: .obj files', createIgnoreTest, null, 'ignore.obj')
util.testSinglePlatform('ignore test: string in array', createIgnoreTest, ['ignorethis'],
                        'ignorethis.txt')
util.testSinglePlatform('ignore test: string', createIgnoreTest, 'ignorethis', 'ignorethis.txt')
util.testSinglePlatform('ignore test: RegExp', createIgnoreTest, /ignorethis/, 'ignorethis.txt')
util.testSinglePlatform('ignore test: Function', createIgnoreTest,
                        file => { return file.match(/ignorethis/) }, 'ignorethis.txt')
util.testSinglePlatform('ignore test: string with slash', createIgnoreTest, 'ignore/this',
  path.join('ignore', 'this.txt'))
util.testSinglePlatform('ignore test: only match subfolder of app', createIgnoreTest,
                        'electron-packager', path.join('electron-packager', 'readme.txt'))
util.testSinglePlatform('ignore out dir test', createIgnoreOutDirTest, 'ignoredOutDir')
util.testSinglePlatform('ignore out dir test: unnormalized path', createIgnoreOutDirTest,
                        './ignoredOutDir')
util.testSinglePlatform('ignore out dir test: unnormalized path', createIgnoreImplicitOutDirTest)
