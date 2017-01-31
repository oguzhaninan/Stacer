'use strict'

const common = require('./common')
const debug = require('debug')('electron-packager')
const fs = require('fs-extra')
const path = require('path')
const plist = require('plist')
const series = require('run-series')
const sign = require('electron-osx-sign')

class MacApp {
  constructor (opts, stagingPath) {
    this.opts = opts
    this.stagingPath = stagingPath
    this.appName = opts.name
    this.operations = []
    this.renamedAppPath = path.join(this.stagingPath, `${common.sanitizeAppName(this.appName)}.app`)
    this.electronAppPath = path.join(this.stagingPath, 'Electron.app')
    this.contentsPath = path.join(this.electronAppPath, 'Contents')
    this.frameworksPath = path.join(this.contentsPath, 'Frameworks')
    this.resourcesPath = path.join(this.contentsPath, 'Resources')
  }

  get appCategoryType () {
    return this.opts['app-category-type']
  }

  get appCopyright () {
    return this.opts['app-copyright']
  }

  get appVersion () {
    return this.opts['app-version']
  }

  get buildVersion () {
    return this.opts['build-version']
  }

  get protocols () {
    return this.opts.protocols.map((protocol) => {
      return {
        CFBundleURLName: protocol.name,
        CFBundleURLSchemes: [].concat(protocol.schemes)
      }
    })
  }

  updatePlist (base, displayName, identifier, name) {
    return Object.assign(base, {
      CFBundleDisplayName: displayName,
      CFBundleExecutable: common.sanitizeAppName(displayName),
      CFBundleIdentifier: identifier,
      CFBundleName: common.sanitizeAppName(name)
    })
  }

  updateHelperPlist (base, suffix) {
    let helperSuffix, identifier, name
    if (suffix) {
      helperSuffix = `Helper ${suffix}`
      identifier = `${this.helperBundleIdentifier}.${suffix}`
      name = `${this.appName} ${helperSuffix}`
    } else {
      helperSuffix = 'Helper'
      identifier = this.helperBundleIdentifier
      name = this.appName
    }
    return this.updatePlist(base, `${this.appName} ${helperSuffix}`, identifier, name)
  }

  extendAppPlist (props) {
    let extendedAppPlist = props

    if (typeof props === 'string') {
      extendedAppPlist = plist.parse(fs.readFileSync(props).toString())
    }

    return Object.assign(this.appPlist, extendedAppPlist)
  }

  loadPlist (filename) {
    return plist.parse(fs.readFileSync(filename).toString())
  }

  ehPlistFilename (helper) {
    return path.join(this.frameworksPath, helper, 'Contents', 'Info.plist')
  }

  updatePlistFiles () {
    let appPlistFilename = path.join(this.contentsPath, 'Info.plist')
    let helperPlistFilename = this.ehPlistFilename('Electron Helper.app')
    let helperEHPlistFilename = this.ehPlistFilename('Electron Helper EH.app')
    let helperNPPlistFilename = this.ehPlistFilename('Electron Helper NP.app')
    this.appPlist = this.loadPlist(appPlistFilename)
    let helperPlist = this.loadPlist(helperPlistFilename)
    let helperEHPlist = this.loadPlist(helperEHPlistFilename)
    let helperNPPlist = this.loadPlist(helperNPPlistFilename)

    let defaultBundleName = `com.electron.${common.sanitizeAppName(this.appName).toLowerCase()}`

    let appBundleIdentifier = filterCFBundleIdentifier(this.opts['app-bundle-id'] || defaultBundleName)
    this.helperBundleIdentifier = filterCFBundleIdentifier(this.opts['helper-bundle-id'] || `${appBundleIdentifier}.helper`)

    if (this.opts['extend-info']) {
      this.appPlist = this.extendAppPlist(this.opts['extend-info'])
    }

    this.appPlist = this.updatePlist(this.appPlist, this.appName, appBundleIdentifier, this.appName)
    helperPlist = this.updateHelperPlist(helperPlist)
    helperEHPlist = this.updateHelperPlist(helperEHPlist, 'EH')
    helperNPPlist = this.updateHelperPlist(helperNPPlist, 'NP')

    if (this.appVersion) {
      this.appPlist.CFBundleShortVersionString = this.appPlist.CFBundleVersion = '' + this.appVersion
    }

    if (this.buildVersion) {
      this.appPlist.CFBundleVersion = '' + this.buildVersion
    }

    if (this.opts.protocols && this.opts.protocols.length) {
      this.appPlist.CFBundleURLTypes = this.protocols
    }

    if (this.appCategoryType) {
      this.appPlist.LSApplicationCategoryType = this.appCategoryType
    }

    if (this.appCopyright) {
      this.appPlist.NSHumanReadableCopyright = this.appCopyright
    }

    fs.writeFileSync(appPlistFilename, plist.build(this.appPlist))
    fs.writeFileSync(helperPlistFilename, plist.build(helperPlist))
    fs.writeFileSync(helperEHPlistFilename, plist.build(helperEHPlist))
    fs.writeFileSync(helperNPPlistFilename, plist.build(helperNPPlist))
  }

  moveHelpers (callback) {
    series([' Helper', ' Helper EH', ' Helper NP'].map((suffix) => {
      return (cb) => {
        let executableBasePath = path.join(this.frameworksPath, `Electron${suffix}.app`, 'Contents', 'MacOS')

        common.rename(executableBasePath, `Electron${suffix}`, `${common.sanitizeAppName(this.appName)}${suffix}`, (err) => {
          if (err) return cb(err)
          common.rename(this.frameworksPath, `Electron${suffix}.app`, `${common.sanitizeAppName(this.appName)}${suffix}.app`, cb)
        })
      }
    }), (err) => {
      callback(err)
    })
  }

  enqueueCopyingIcon () {
    this.operations.push((cb) => {
      common.normalizeExt(this.opts.icon, '.icns', (err, icon) => {
        if (err) {
          // Ignore error if icon doesn't exist, in case it's only available for other OS
          cb(null)
        } else {
          debug(`Copying icon "${icon}" to app's Resources as "${this.appPlist.CFBundleIconFile}"`)
          fs.copy(icon, path.join(this.resourcesPath, this.appPlist.CFBundleIconFile), cb)
        }
      })
    })
  }

  enqueueCopyingExtraFiles (extras) {
    if (!Array.isArray(extras)) extras = [extras]
    extras.forEach((filename) => {
      this.operations.push((cb) => {
        fs.copy(filename, path.join(this.resourcesPath, path.basename(filename)), cb)
      })
    })
  }

  enqueueRenamingElectronBinary () {
    this.operations.push((cb) => {
      common.rename(path.join(this.contentsPath, 'MacOS'),
                    'Electron',
                    this.appPlist.CFBundleExecutable,
                    cb)
    })
  }

  enqueueRenamingAppAndHelpers () {
    this.operations.push((cb) => {
      this.moveHelpers(cb)
    }, (cb) => {
      fs.rename(this.electronAppPath, this.renamedAppPath, cb)
    })
  }

  enqueueAppSigningIfSpecified () {
    let osxSignOpt = this.opts['osx-sign']
    let platform = this.opts.platform
    let version = this.opts.electronVersion

    if ((platform === 'all' || platform === 'mas') &&
        osxSignOpt === undefined) {
      common.warning('WARNING: signing is required for mas builds. Provide the osx-sign option, ' +
                     'or manually sign the app later.')
    }

    if (osxSignOpt) {
      this.operations.push((cb) => {
        let signOpts = createSignOpts(osxSignOpt, platform, this.renamedAppPath, version, this.opts.quiet)
        debug(`Running electron-osx-sign with the options ${JSON.stringify(signOpts)}`)
        sign(signOpts, (err) => {
          if (err) {
            // Although not signed successfully, the application is packed.
            common.warning('Code sign failed; please retry manually.', err)
          }
          cb()
        })
      })
    }
  }

  executeOperations (callback) {
    series(this.operations, (err) => {
      if (err) return callback(err)
      common.moveApp(this.opts, this.stagingPath, callback)
    })
  }
}

// Remove special characters and allow only alphanumeric (A-Z,a-z,0-9), hyphen (-), and period (.)
// Apple documentation:
// https://developer.apple.com/library/mac/documentation/General/Reference/InfoPlistKeyReference/Articles/CoreFoundationKeys.html#//apple_ref/doc/uid/20001431-102070
function filterCFBundleIdentifier (identifier) {
  return identifier.replace(/ /g, '-').replace(/[^a-zA-Z0-9.-]/g, '')
}

function createSignOpts (properties, platform, app, version, quiet) {
  // use default sign opts if osx-sign is true, otherwise clone osx-sign object
  let signOpts = properties === true ? {identity: null} : Object.assign({}, properties)

  // osx-sign options are handed off to sign module, but
  // with a few additions from the main options
  // user may think they can pass platform, app, or version, but they will be ignored
  common.subOptionWarning(signOpts, 'osx-sign', 'platform', platform, quiet)
  common.subOptionWarning(signOpts, 'osx-sign', 'app', app, quiet)
  common.subOptionWarning(signOpts, 'osx-sign', 'version', version, quiet)

  if (signOpts.binaries) {
    common.warning('WARNING: osx-sign.binaries is not an allowed sub-option. Not passing to electron-osx-sign.')
    delete signOpts.binaries
  }

  // Take argument osx-sign as signing identity:
  // if opts['osx-sign'] is true (bool), fallback to identity=null for
  // autodiscovery. Otherwise, provide signing certificate info.
  if (signOpts.identity === true) {
    signOpts.identity = null
  }

  return signOpts
}

module.exports = {
  createApp: function createApp (opts, templatePath, callback) {
    let appRelativePath = path.join('Electron.app', 'Contents', 'Resources', 'app')
    common.initializeApp(opts, templatePath, appRelativePath, function buildMacApp (err, stagingPath) {
      if (err) return callback(err)

      let appCreator = new MacApp(opts, stagingPath)
      appCreator.updatePlistFiles()

      if (opts.icon) {
        appCreator.enqueueCopyingIcon()
      }

      if (opts['extra-resource']) {
        appCreator.enqueueCopyingExtraFiles(opts['extra-resource'])
      }

      appCreator.enqueueRenamingElectronBinary()
      appCreator.enqueueRenamingAppAndHelpers()
      appCreator.enqueueAppSigningIfSpecified()

      return appCreator.executeOperations(callback)
    })
  },
  createSignOpts: createSignOpts,
  filterCFBundleIdentifier: filterCFBundleIdentifier
}
