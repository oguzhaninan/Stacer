'use strict'

const common = require('../common')
const test = require('tape')

test('CLI argument test: --electron-version populates opts.electronVersion', (t) => {
  let args = common.parseCLIArgs([])
  t.equal(args.electronVersion, undefined)
  args = common.parseCLIArgs(['--electron-version=1.2.3'])
  t.equal(args.electronVersion, '1.2.3')
  t.end()
})

test('CLI argument test: --download.strictSSL default', function (t) {
  var args = common.parseCLIArgs([])
  t.true(args.download.strictSSL, 'default for --download.strictSSL is true')
  t.end()
})

test('CLI argument test: --asar=true', function (t) {
  var args = common.parseCLIArgs(['--asar=true'])
  t.equal(args.asar, true)
  t.end()
})

test('CLI argument test: using --asar overrides other --asar.options', (t) => {
  let args = common.parseCLIArgs(['--asar', '--asar.unpack=*.node'])
  t.equal(args.asar, true)
  args = common.parseCLIArgs(['--asar.unpack=*.node', '--asar'])
  t.equal(args.asar, true)
  t.end()
})

test('CLI argument test: --osx-sign=true', function (t) {
  var args = common.parseCLIArgs(['--osx-sign=true'])
  t.equal(args['osx-sign'], true)
  t.end()
})

test('CLI argument test: --tmpdir=false', function (t) {
  var args = common.parseCLIArgs(['--tmpdir=false'])
  t.equal(args.tmpdir, false)
  t.end()
})

test('CLI argument test: --deref-symlinks default', function (t) {
  var args = common.parseCLIArgs([])
  t.equal(args['deref-symlinks'], true)
  t.end()
})

test('CLI argument test: --out always resolves to a string', (t) => {
  var args = common.parseCLIArgs(['--out=1'])
  t.equal(args.out, '1')
  t.end()
})

test('CLI argument test: --out without a value is the same as not passing --out', (t) => {
  var args = common.parseCLIArgs(['--out'])
  t.equal(args.out, null)
  t.end()
})

test('CLI argument test: --protocol with a corresponding --protocol-name', (t) => {
  var args = common.parseCLIArgs(['--protocol=foo', '--protocol-name=Foo'])
  t.deepEqual(args.protocols, [{schemes: ['foo'], name: 'Foo'}])
  t.end()
})

test('CLI argument test: --protocol without a corresponding --protocol-name', (t) => {
  var args = common.parseCLIArgs(['--protocol=foo'])
  t.deepEqual(args.protocols, undefined)
  t.end()
})

test('CLI argument test: multiple --protocol/--protocol-name argument pairs', (t) => {
  var args = common.parseCLIArgs(['--protocol=foo', '--protocol-name=Foo', '--protocol=bar', '--protocol-name=Bar'])
  t.deepEqual(args.protocols, [{schemes: ['foo'], name: 'Foo'}, {schemes: ['bar'], name: 'Bar'}])
  t.end()
})
