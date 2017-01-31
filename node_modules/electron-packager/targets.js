'use strict'

const common = require('./common')

function unsupportedListOption (name, value, supported) {
  return new Error(`Unsupported ${name}=${value} (${typeof value}); must be a String matching: ${Object.keys(supported).join(', ')}`)
}

module.exports = {
  supportedArchs: common.archs.reduce((result, arch) => {
    result[arch] = true
    return result
  }, {}),
  // Maps to module filename for each platform (lazy-required if used)
  supportedPlatforms: {
    darwin: './mac',
    linux: './linux',
    mas: './mac', // map to darwin
    win32: './win32'
  },

  // Validates list of architectures or platforms.
  // Returns a normalized array if successful, or throws an Error.
  validateListFromOptions: function validateListFromOptions (opts, supported, name) {
    if (opts.all) return Object.keys(supported)

    let list = opts[name] || process[name]
    if (list === 'all') return Object.keys(supported)

    if (!Array.isArray(list)) {
      if (typeof list === 'string') {
        list = list.split(',')
      } else {
        return unsupportedListOption(name, list, supported)
      }
    }

    for (let value of list) {
      if (!supported[value]) {
        return unsupportedListOption(name, value, supported)
      }
    }

    return list
  }
}
