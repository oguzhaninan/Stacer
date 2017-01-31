# Contributing to Shifty

First of all, thanks!  Community contribution is what makes open source great.
If you find a bug or would like to make a feature request, please report it on
the [issue tracker](https://github.com/jeremyckahn/shifty/issues).  If you
would like to make changes to the code yourself, read on!

## Getting started

To get started with hacking on Shifty, you'll need to get all of the
development dependencies with [npm](https://npmjs.org/) (and, by extension,
[Node](http://nodejs.org/)):

````
$: npm install
````

## Pull Requests and branches

The project maintainer ([@jeremyckahn](https://github.com/jeremyckahn)) manages
releases.  `master` contains the latest stable code, and `develop` contains
commits that are ahead of (newer than) `master` that have yet to be officially
released (built and tagged).  When making a Pull Request, please branch off of
`develop` and request to merge back into it.  `master` is only merged into from
`develop`.

## Versioning

Shifty uses [SemVer](http://semver.org/) for versioning.  If you modify the
source code, please adhere to this convention (in all likelihood you will only
need to modify the rightmost digit by one).  To change the version, you'll need
to update the version in two places: `bower.json` and `package.json` (look for
the lines that say `version`).  The version numbers in these two files must be
kept in sync.

## Building

Shifty uses [Grunt](http://gruntjs.com/) (version 0.4.1 or later, globally
installed) to generate the distributable binaries.  If you make any changes to
the source code and need to rebuild the binaries for testing or an alternative
release, run this command:

````
$: grunt build
````

## Testing

Please make sure that all tests pass before submitting a Pull Request.  To run
the tests on the command line (requires [PhantomJS](http://phantomjs.org/)):

````
$: grunt qunit
````

You can also run the tests in the browser.  They are in `tests/`.  If you are
adding a feature or fixing a bug, please add a test!

## Style

Please try to remain consitent with existing code.  To automatically check for
style issues or other potential problems, you can run:

````
$: grunt jshint
````
