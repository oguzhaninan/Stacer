# electron-packager

Package your [Electron](http://electron.atom.io) app into OS-specific bundles (`.app`, `.exe`, etc.) via JavaScript or the command line.

[![Travis CI Build Status](https://travis-ci.org/electron-userland/electron-packager.svg?branch=master)](https://travis-ci.org/electron-userland/electron-packager)
[![AppVeyor Build status](https://ci.appveyor.com/api/projects/status/m51mlf6ntd138555?svg=true)](https://ci.appveyor.com/project/electron-userland/electron-packager)
[![Coverage Status](https://coveralls.io/repos/github/electron-userland/electron-packager/badge.svg?branch=master)](https://coveralls.io/github/electron-userland/electron-packager?branch=master)
[![Dependency Status](https://dependencyci.com/github/electron-userland/electron-packager/badge)](https://dependencyci.com/github/electron-userland/electron-packager)

## About

Electron Packager is a command line tool and Node.js library that bundles Electron-based application
source code with a renamed Electron executable and supporting files into folders ready for distribution.

Note that packaged Electron applications can be relatively large. A zipped barebones OS X Electron application is around 40MB.

### Electron Packager is an [OPEN Open Source Project](http://openopensource.org/)

Individuals making significant and valuable contributions are given commit-access to the project to contribute as they see fit. This project is more like an open wiki than a standard guarded open source project.

See [CONTRIBUTING.md](https://github.com/electron-userland/electron-packager/blob/master/CONTRIBUTING.md) and [openopensource.org](http://openopensource.org/) for more details.

### [Release Notes](https://github.com/electron-userland/electron-packager/blob/master/NEWS.md)

## Supported Platforms

Electron Packager is known to run on the following **host** platforms:

* Windows (32/64 bit)
* OS X
* Linux (x86/x86_64)

It generates executables/bundles for the following **target** platforms:

* Windows (also known as `win32`, for both 32/64 bit)
* OS X (also known as `darwin`) / [Mac App Store](http://electron.atom.io/docs/v0.36.0/tutorial/mac-app-store-submission-guide/) (also known as `mas`)<sup>*</sup>
* Linux (for x86, x86_64, and armv7l architectures)

<sup>*</sup> *Note for OS X / MAS target bundles: the `.app` bundle can only be signed when building on a host OS X platform.*

## Installation

This module requires Node.js 4.0 or higher to run.

```sh
# for use in npm scripts
npm install electron-packager --save-dev

# for use from cli
npm install electron-packager -g
```

### Building Windows apps from non-Windows platforms

Building an Electron app for the Windows target platform requires editing the `Electron.exe` file.
Currently, Electron Packager uses [node-rcedit](https://github.com/atom/node-rcedit) to accomplish
this. A Windows executable is bundled in that Node package and needs to be run in order for this
functionality to work, so on non-Windows host platforms, [Wine](https://www.winehq.org/) 1.6 or
later needs to be installed. On OS X, it is installable via [Homebrew](http://brew.sh/).

## Usage

### From the Command Line

Running electron-packager from the command line has this basic form:

```
electron-packager <sourcedir> <appname> --platform=<platform> --arch=<arch> [optional flags...]
```

This will:

- Find or download the correct release of Electron
- Use that version of Electron to create a app in `<out>/<appname>-<platform>-<arch>` *(this can be customized via an optional flag)*

`--platform` and `--arch` can be omitted, in two cases:

* If you specify `--all` instead, bundles for all valid combinations of target
  platforms/architectures will be created.
* Otherwise, a single bundle for the host platform/architecture will be created.

For an overview of the other optional flags, run `electron-packager --help` or see
[usage.txt](https://github.com/electron-userland/electron-packager/blob/master/usage.txt). For
detailed descriptions, see the [API documentation](https://github.com/electron-userland/electron-packager/blob/master/docs/api.md).

If `appname` is omitted, this will use the name specified by "productName" or "name" in the nearest package.json.

**Characters in the Electron app name which are not allowed in all target platforms' filenames
(e.g., `/`), will be replaced by hyphens (`-`).**

You should be able to launch the app on the platform you built for. If not, check your settings and try again.

**Be careful** not to include `node_modules` you don't want into your final app. If you put them in
the `devDependencies` section of `package.json`, by default none of the modules related to those
dependencies will be copied in the app bundles. (This behavior can be turned off with the
`--no-prune` flag.) In addition, folders like `.git` and `node_modules/.bin` will be ignored by
default. You can use `--ignore` to ignore files and folders via a regular expression (*not* a
[glob pattern](https://en.wikipedia.org/wiki/Glob_%28programming%29)). Examples include
`--ignore=\.gitignore` or `--ignore="\.git(ignore|modules)"`.

#### Example

Let's assume that you have made an app based on the [electron-quick-start](https://github.com/electron/electron-quick-start) repository on a OS X or Linux host platform with the following file structure:

```
foobar
├── package.json
├── index.html
├── […other files, like LICENSE…]
└── script.js
```

…and that the following is true:

* `electron-packager` is installed globally
* `productName` in `package.json` has been set to `Foo Bar`
* `npm install` for the `Foo Bar` app has been run at least once

When one runs the following command for the first time in the `foobar` directory:

```
electron-packager . --all
```

`electron-packager` will do the following:

* Use the current directory for the `sourcedir`
* Infer the `appname` from the `productName` in `package.json`
* Infer the `app-version` from the `version` in `package.json`
* Download [all supported target platforms and arches](#supported-platforms) of Electron
using the installed `electron` or `electron-prebuilt` version (and cache the downloads in `~/.electron`)
* For the `darwin` build, as an example:
  * build the OS X `Foo Bar.app`
  * place `Foo Bar.app` in `foobar/Foo Bar-darwin-x64/` (since an `out` directory was not specified, it used the current working directory)

The file structure now looks like:

```
foobar
├── Foo Bar-darwin-x64
│   ├── Foo Bar.app
│   │   └── […Mac app contents…]
│   ├── LICENSE
│   └── version
├── […other application bundles, like "Foo Bar-win32-x64" (sans quotes)…]
├── package.json
├── index.html
├── […other files, like LICENSE…]
└── script.js
```

The `Foo Bar.app` folder generated can be executed by a system running OS X, which will start the packaged Electron app. This is also true of the Windows x64 build on a system running a new enough version of Windows for a 64-bit system (via `Foo Bar-win32-x64/Foo Bar.exe`), and so on.

### [Programmatic API](https://github.com/electron-userland/electron-packager/blob/master/docs/api.md)

## Related

- [Electron Forge](https://www.npmjs.com/package/electron-forge) - creates, builds, and distributes modern Electron applications
- [electron-packager-interactive](https://github.com/Urucas/electron-packager-interactive) - an interactive CLI for electron-packager
- [electron-packager-plugin-non-proprietary-codecs-ffmpeg](https://www.npmjs.com/package/electron-packager-plugin-non-proprietary-codecs-ffmpeg) - replaces the normal version of FFmpeg in Electron with a version without proprietary codecs
- [electron-rebuild](https://github.com/electron/electron-rebuild) - rebuild native Node.js modules
  against the packaged Electron version
- [grunt-electron](https://github.com/sindresorhus/grunt-electron) - grunt plugin for electron-packager
