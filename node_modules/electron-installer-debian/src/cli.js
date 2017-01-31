#!/usr/bin/env node

var _ = require('lodash')
var yargs = require('yargs')

var installer = require('./installer')
var pkg = require('../package.json')

var argv = yargs
  .version(pkg.version)
  .usage(pkg.description + '\n\nUsage: $0 --src <inputdir> --dest <outputdir> --arch <architecture>')
  .option('src', {
    describe: 'Directory that contains your built Electron app (e.g. with `electron-packager`)',
    demand: true
  })
  .option('dest', {
    describe: 'Directory that will contain the resulting Debian installer',
    demand: true
  })
  .option('arch', {
    describe: 'Machine architecture the package is targeted to',
    demand: true
  })
  .option('config', {
    describe: 'JSON file that contains the metadata for your application',
    config: true
  })
  .example('$0 --src dist/app/ --dest dist/installer/ --arch i386', 'use metadata from `dist/app/`')
  .example('$0 --src dist/app/ --dest dist/installer/ --config config.json', 'use metadata from `config.json`')
  .wrap(null)
  .argv

console.log('Creating package (this may take a while)')

var options = _.omit(argv, ['$0', '_', 'version'])
installer(options, function (err) {
  if (err) {
    console.error(err, err.stack)
    process.exit(1)
  }

  console.log('Successfully created package at ' + argv.dest)
})
