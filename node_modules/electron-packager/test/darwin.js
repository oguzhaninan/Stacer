'use strict'

const config = require('./config.json')
const path = require('path')

var baseOpts = {
  name: 'basicTest',
  dir: path.join(__dirname, 'fixtures', 'basic'),
  electronVersion: config.version,
  arch: 'x64',
  platform: 'darwin'
}

require('./mac')(baseOpts)
