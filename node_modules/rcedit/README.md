# node-rcedit [![Travis Build Status](https://travis-ci.org/electron/node-rcedit.svg?branch=master)](https://travis-ci.org/electron/node-rcedit) [![AppVeyor Build Status](https://ci.appveyor.com/api/projects/status/515pyecj579vuqe8/branch/master?svg=true)](https://ci.appveyor.com/project/Atom/node-rcedit/branch/master)


Node module to edit resources of Windows executables.

## Building

* Clone the repository
* Run `npm install`
* Run `npm test` to run the tests

## Docs

```js
var rcedit = require('rcedit')
```
On platforms other than Windows, you will need to have [Wine](http://winehq.org)
1.6 or later installed and in the system path.

### `rcedit(exePath, options, callback)`

`exePath` is the path to the Windows executable to be modified.

`options` is an object that can contain following fields:

* `version-string` - An object containing properties to change the `exePath`'s
  version string.
* `file-version` - File's version to change to.
* `product-version` - Product's version to change to.
* `icon` - Path to the icon file (`.ico`) to set as the `exePath`'s default icon.

`callback` is the `Function` called when the command completes. The function
signature is `function (error)`.
