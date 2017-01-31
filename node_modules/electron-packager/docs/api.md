# electron-packager API

Short example:

```javascript
var packager = require('electron-packager')
packager(options, function done_callback (err, appPaths) { /* … */ })
```

## `options`

### Required

#### `dir`

*String*

The source directory.

### Optional

#### All Platforms

##### `afterCopy`

*Array of Functions*

An array of functions to be called after your app directory has been copied to a temporary directory.  Each function is called with five parameters:

- `buildPath` (*String*): The path to the temporary folder where your app has been copied to
- `electronVersion` (*String*): The version of electron you are packaging for
- `platform` (*String*): The target platform you are packaging for
- `arch` (*String*): The target architecture you are packaging for
- `callback` (*Function*): Must be called once you have completed your actions

##### `afterExtract`

*Array of Functions*

An array of functions to be called after Electron has been extracted to a temporary directory.  Each function is called with five parameters:

- `buildPath` (*String*): The path to the temporary folder where Electron has been extracted to
- `electronVersion` (*String*): The version of electron you are packaging for
- `platform` (*String*): The target platform you are packaging for
- `arch` (*String*): The target architecture you are packaging for
- `callback` (*Function*): Must be called once you have completed your actions

##### `all`

*Boolean*

When `true`, sets both [`arch`](#arch) and [`platform`](#platform) to `all`.

##### `app-copyright`

*String*

The human-readable copyright line for the app. Maps to the `LegalCopyright` metadata property on Windows, and `NSHumanReadableCopyright` on OS X.

##### `app-version`

*String*

The release version of the application. By default the `version` property in the `package.json` is used but it can be overridden with this argument. If neither are provided, the version of Electron will be used. Maps to the `ProductVersion` metadata property on Windows, and `CFBundleShortVersionString` on OS X.

#### `arch`

*String* (default: the arch of the host computer running Node)

Allowed values: `ia32`, `x64`, `armv7l`, `all`

The target system architecture(s) to build for.
Not required if the [`all`](#all) option is set.
If `arch` is set to `all`, all supported architectures for the target platforms specified by [`platform`](#platform) will be built.
Arbitrary combinations of individual architectures are also supported via a comma-delimited string or array of strings.
The non-`all` values correspond to the architecture names used by [Electron releases].

##### `asar`

*Boolean* or *Object* (default: `false`)

Whether to package the application's source code into an archive, using [Electron's archive format](https://github.com/electron/asar). Reasons why you may want to enable this feature are described in [an application packaging tutorial in Electron's documentation](http://electron.atom.io/docs/v0.36.0/tutorial/application-packaging/). When the value is `true`, pass default configuration to the `asar` module. The configuration values listed below can be customized when the value is an `Object`. Supported parameters include, but are not limited to:
- `ordering` (*String*): A path to an ordering file for packing files. An explanation can be found on the [Atom issue tracker](https://github.com/atom/atom/issues/10163).
- `unpack` (*String*): A [glob expression](https://github.com/isaacs/minimatch#features), when specified, unpacks the file with matching names to the `app.asar.unpacked` directory.
- `unpackDir` (*String*): Unpacks the dir to the `app.asar.unpacked` directory whose names exactly or pattern match this string. The `asar.unpackDir` is relative to `dir`.

  Some examples:

  - `asar.unpackDir = 'sub_dir'` will unpack the directory `/<dir>/sub_dir`
  - `asar.unpackDir = '**/{sub_dir1/sub_sub_dir,sub_dir2}/*'` will unpack the directories `/<dir>/sub_dir1/sub_sub_dir` and `/<dir>/sub_dir2`, but it will not include their subdirectories.
  - `asar.unpackDir = '**/{sub_dir1/sub_sub_dir,sub_dir2}/**'` will unpack the subdirectories of the directories `/<dir>/sub_dir1/sub_sub_dir` and `/<dir>/sub_dir2`.
  - `asar.unpackDir = '**/{sub_dir1/sub_sub_dir,sub_dir2}/**/*'` will unpack the directories `/<dir>/sub_dir1/sub_sub_dir` and `/<dir>/sub_dir2` and their subdirectories.

##### `build-version`

*String*

The build version of the application. Defaults to the value of [`app-version`](#app-version). Maps to the `FileVersion` metadata property on Windows, and `CFBundleVersion` on OS X.

##### `derefSymlinks`

*Boolean* (default: `true`)

Whether symlinks should be dereferenced during copying.

##### `download`

*Object*

If present, passes custom options to [`electron-download`](https://www.npmjs.com/package/electron-download)
(see the link for more detailed option descriptions and the defaults). Supported parameters include,
but are not limited to:
- `cache` (*String*): The directory where prebuilt, pre-packaged Electron downloads are cached.
- `mirror` (*String*): The URL to override the default Electron download location.
- `quiet` (*Boolean* - default: `false`): Whether to show a progress bar when downloading Electron.
- `strictSSL` (*Boolean* - default: `true`): Whether SSL certificates are required to be valid when
  downloading Electron.

##### `electronVersion`

*String*

The Electron version with which the app is built (without the leading 'v') - for example,
[`1.4.13`](https://github.com/electron/electron/releases/tag/v1.4.13). See [Electron releases] for
valid versions. If omitted, it will use the version of the nearest local installation of
`electron` or `electron-prebuilt`, defined in `package.json` in either `dependencies` or `devDependencies`.

##### `icon`

*String*

The local path to the icon file, if the target platform supports setting embedding an icon.

Currently you must look for conversion tools in order to supply an icon in the format required by the platform:

- OS X: `.icns`
- Windows: `.ico` ([See the readme](https://github.com/electron-userland/electron-packager#building-windows-apps-from-non-windows-platforms) for details on non-Windows platforms)
- Linux: this option is not required, as the dock/window list icon is set via
  [the `icon` option in the `BrowserWindow` constructor](http://electron.atom.io/docs/api/browser-window/#new-browserwindowoptions).
  *Please note that you need to use a PNG, and not the OS X or Windows icon formats, in order for it
  to show up in the dock/window list.* Setting the icon in the file manager is not currently supported.

If the file extension is omitted, it is auto-completed to the correct extension based on the platform, including when [`--platform=all`](#platform) is in effect.

##### `ignore`

*RegExp*, *Array* of *RegExp*s, or *Function*

One or more additional [regular expression](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Regular_Expressions)
patterns which specify which files to ignore when copying files to create the app bundle(s). The
regular expressions are matched against the absolute path of a given file/directory to be copied.

The following paths are always ignored (*when you aren't using the predicate function that is
described after the list*):

* the directory specified by the [`out`](#out) parameter
* `node_modules/.bin`
* `node_modules/electron`
* `node_modules/electron-prebuilt`
* `node_modules/electron-packager`
* `.git`
* files and folders ending in `.o` and `.obj`

**Please note that [glob patterns](https://en.wikipedia.org/wiki/Glob_%28programming%29) will not work.**

**Note**: If you're trying to ignore Node modules specified in `devDependencies`, you may want to
use [`prune`](#prune) instead.

Alternatively, this can be a predicate function that, given an absolute file path, returns `true` if
the file should be ignored, or `false` if the file should be kept. *This does not use any of the
default ignored directories listed above.*

##### `name`

*String*

The application name. If omitted, it will use the `productName` or `name` value from the nearest `package.json`.

**Regardless of source, characters in the Electron app name which are not allowed in all target
platforms' filenames (e.g., `/`), will be replaced by hyphens (`-`).**

##### `out`

*String* (default: current working directory)

The base directory where the finished package(s) are created.

##### `overwrite`

*Boolean* (default: `false`)

Whether to replace an already existing output directory for a given platform (`true`) or skip recreating it (`false`).

#### `platform`

*String* (default: the arch of the host computer running Node)

Allowed values: `linux`, `win32`, `darwin`, `mas`, `all`

The target platform(s) to build for.
Not required if the [`all`](#all) option is set.
If `platform` is set to `all`, all [supported target platforms](#supported-platforms) for the target architectures specified by [`arch`](#arch) will be built.
Arbitrary combinations of individual platforms are also supported via a comma-delimited string or array of strings.
The non-`all` values correspond to the platform names used by [Electron releases].

##### `prune`

*Boolean* (default: `true`)

Runs [`npm prune --production`](https://docs.npmjs.com/cli/prune) before starting to package the app.

##### `quiet`

*Boolean* (default: `false`)

If `true`, disables printing informational and warning messages to the console when packaging the
application. This does *not* disable errors.

##### `tmpdir`

*String* or *`false`* (default: system temp directory)

The base directory to use as a temp directory. Set to `false` to disable use of a temporary directory.

##### `version`

*String* (**deprecated** and will be removed in a future major version, please use the
[`electronVersion`](#electronversion) parameter instead)

#### OS X/Mac App Store targets only

##### `app-bundle-id`

*String*

The bundle identifier to use in the application's plist.

##### `app-category-type`

*String*

The application category type, as shown in the Finder via *View → Arrange by Application Category* when viewing the Applications directory.

For example, `app-category-type=public.app-category.developer-tools` will set the application category to *Developer Tools*.

Valid values are listed in [Apple's documentation](https://developer.apple.com/library/ios/documentation/General/Reference/InfoPlistKeyReference/Articles/LaunchServicesKeys.html#//apple_ref/doc/uid/TP40009250-SW8).

##### `extend-info`

*String* or *Object*

When the value is a `String`, the filename of a plist file. Its contents are added to the app's plist. When the value is an `Object`, an already-parsed plist data structure that is merged into the app's plist.

Entries from `extend-info` override entries in the base plist file supplied by `electron` or `electron-prebuilt`, but are overridden by other explicit arguments such as [`app-version`](#app-version) or [`app-bundle-id`](#app-bundle-id).



##### `extra-resource`

*String* or *Array*

Filename of a file to be copied directly into the app's `Contents/Resources` directory.

##### `helper-bundle-id`

*String*

The bundle identifier to use in the application helper's plist.

##### `osx-sign`

*Object* or *`true`*

If present, signs OS X target apps when the host platform is OS X and XCode is installed. When the value is `true`, pass default configuration to the signing module. The configuration values listed below can be customized when the value is an `Object`. See [electron-osx-sign](https://www.npmjs.com/package/electron-osx-sign#opts) for more detailed option descriptions and the defaults.
- `identity` (*String*): The identity used when signing the package via `codesign`.
- `entitlements` (*String*): The path to the 'parent' entitlements.
- `entitlements-inherit` (*String*): The path to the 'child' entitlements.

##### `protocol`

*Array* of *String*s

The URL protocol scheme(s) to associate the app with. For example, specifying
`myapp` would cause URLs such as `myapp://path` to be opened with the app. Maps
to the `CFBundleURLSchemes` metadata property. This option requires a
corresponding `protocol-name` option to be specified.

##### `protocol-name`

*Array* of *String*s

The descriptive name(s) of the URL protocol scheme(s) specified via the `protocol`
option. Maps to the `CFBundleURLName` metadata property.

#### Windows targets only

##### `version-string`

*Object* (**deprecated** and will be removed in a future major version, please use the
[`win32metadata`](#win32metadata) parameter instead)

##### `win32metadata`

*Object*

Object (also known as a "hash") of application metadata to embed into the executable:
- `CompanyName`
- `FileDescription`
- `OriginalFilename`
- `ProductName`
- `InternalName`

## callback

### `err`

*Error* (or *Array*, in the case of an `copy` error)

Contains errors, if any.

### `appPaths`

*Array* of *String*s

Paths to the newly created application bundles.

[Electron releases]: https://github.com/electron/electron/releases
