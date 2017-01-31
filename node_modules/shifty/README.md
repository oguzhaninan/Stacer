# Shifty - A teeny tiny tweening engine in JavaScript

Shifty is a tweening engine for JavaScript.  It is a lightweight library meant
to be encapsulated by higher level tools.  At its core, Shifty provides:

  * Interpolation of `Number`s over time (tweening)
  * Playback control of an individual tween
  * Extensibility hooks for key points in the tweening process

This is useful because it is the least amount of functionality needed to build
customizable animations. Shifty is optimized to run many times a second with
minimal processing and memory overhead, which is necessary to achieve smooth
animations.  The core Shifty library doesn't do:

  * Rendering (such as CSS or Canvas)
  * Sequencing
  * Much else

But don't worry!  If you need functionality like this, you can easily extend
the core with whatever you need.  In fact, there are some extensions included
in the standard distribution (the `dist/` directory) that make Shifty more
useful for common animation needs.  Extensions included in the default build
are:

* [`shifty.token.js`](http://jeremyckahn.github.io/shifty/dist/doc/src/shifty.token.js.html):
  String support.  Allows you to interpolate numbers within
  arbitrarily-formatted strings such as "25px" or "rgb(255, 0, 255)".  In other
  words, this extension allows Shifty to tween CSS properties.
* [`shifty.interpolate.js`](http://jeremyckahn.github.io/shifty/dist/doc/src/shifty.interpolate.js.html):
  Compute the midpoint between a set of values outside of a tween.  In other
  words, compute a single frame of an animation.
* [`shifty.bezier.js`](http://jeremyckahn.github.io/shifty/dist/doc/src/shifty.bezier.js.html):
  Define custom easing curves based on a [cubic Bezier
  curve](http://en.wikipedia.org/wiki/B%C3%A9zier_curve#Cubic_B.C3.A9zier_curves).
  Take a look at [cubic-bezier.com](http://cubic-bezier.com/) for a visual aid.
* [`shifty.formulas.js`](src/shifty.formulas.js): A bunch of [Robert
  Penner](http://robertpenner.com/) easing formulas adapted from
  [Scripty2](https://github.com/madrobby/scripty2).

## Installation

Shifty is available through NPM and Bower:

````
npm install --save shifty
````

````
bower install --save shifty
````

## Browser compatibility

Shifty officially supports Internet Explorer 8 and up, as well as all Evergreen
browsers.

## Using Shifty

Shifty has no dependencies, so you can just load
[shifty.min.js](dist/shifty.min.js) and start using it.  This file has all of
the extensions described above baked in.  If you only want the core `Number`
tweening functionality ([shifty.core.js](src/shifty.core.js)), you can easily
build that without any extensions (please see [Building
Shifty](#building-shifty)).

## Getting started

This section explains how to get started with Shifty.  For full documentation
on each method, please see [the API
documentation](http://jeremyckahn.github.io/shifty/dist/doc/classes/Tweenable.html).

## Making a `tweenable()` instance

The first thing you need to do is create a `new` instance of `Tweenable`:

````javascript
var tweenable = new Tweenable();
````

Optionally, you can also define the initial state of the `Tweenable` instance
to the constructor via a configuration Object:

````javascript
var tweenable = new Tweenable({
  x: 50,
  y: 100,
  opacity: 0.5
});
````

Supplying the initial state to the constructor would obviate the need to supply
a `from` value to the `tween` method.

## tween

Make a basic tween by specifying some options:

  * from (Object): Starting position.  Required.
  * to (Object): Ending position (signature must match `from`). Required.
  * duration (number): How long to animate for.
  * easing (string): Easing formula name to use for tween.
  * start (function): Function to execute when the tween begins (after the
    first tick).
  * step (function): Function to execute every tick.
  * finish (function): Function to execute upon completion.

````javascript
var tweenable = new Tweenable();

tweenable.tween({
  from: { x: 0,  y: 50  },
  to:   { x: 10, y: -30 },
  duration: 1500,
  easing: 'easeOutQuad',
  start: function () { console.log('Off I go!'); },
  finish: function () { console.log('And I\'m done!'); }
});
````

## Easing formulas

Shifty supports a number of easing formulas, which you can see in
[`shifty.formulas.js`](src/shifty.formulas.js).  You can add new easing
formulas by attaching methods to `Tweenable.prototype.formula`.

## Advanced usage

You can control the state of a tween with the following methods:

````javascript
Tweenable.prototype.stop();
Tweenable.prototype.pause();
Tweenable.prototype.resume();
````

You can also examine and modify the state of a `Tweenable`:

````javascript
Tweenable.prototype.get();
Tweenable.prototype.set();
````

These, as well as all other methods, are detailed more in the [API
documentation](http://jeremyckahn.github.io/shifty/dist/doc/classes/Tweenable.html).

## Using multiple easing formulas

Shifty supports tweens that have different easing formulas for each property.
Having multiple easing formulas on a single tween can make for some really
interesting animations, because you aren't constrained to moving things in a
straight line.  You can make curves!  To do this, simply supply `easing` as an
Object, rather than a string to `tween()`:

````javascript
var tweenable = new Tweenable();

tweenable.tween({
  from: {
    x: 0,
    y: 0
  },
  to: {
    x: 250,
    y: 150
  },
  easing: {
    x: 'swingFromTo',
    y: 'bounce'
  }
});
````

The Interpolate extension also supports both string and object parameter types
for `easing`.

## Per-tween custom easing functions

You are not limited to attaching functions to `Tweenable.prototype.formula`.
You can also supply a custom easing curve directly to a `tween()` call:

````javascript
var tweenable = new Tweenable();
var easingFunction = function (pos) {
  // This sample function is the same as easeInQuad
  return Math.pow(pos, 2);
};

tweenable.tween({
  from: {
    x: 0,
    y: 0
  },
  to: {
    x: 250,
    y: 150
  },
  easing: easingFunction
});
````

Or even an Object of mixed strings and functions:

````javascript
tweenable.tween({
  from: {
    x: 0,
    y: 0
  },
  to: {
    x: 250,
    y: 150
  },
  easing: {
    x: easingFunction,
    y: 'linear'
  }
});
````

## Filters

Filters are used for transforming the properties of a tween at various points
in a `Tweenable` instance's life cycle.  Filters are meant to convert
non-`Number` data types to `Number`s so they can be tweened, and then back
again. Just define a filter once, attach it to `Tweenable.prototype`, and all
`new` instances of `Tweenable` will have access to it.

Here's an annotated example of a filter:

````javascript
Tweenable.prototype.filter.doubler = {
  // Gets called when a tween is created.
  //
  // `currentState` is the current state of the tweened object, `fromState` is
  // the state that the tween started at, and `toState` contains the target
  // values.
  'tweenCreated': function tweenCreated (currentState, fromState, toState) {
    Tweenable.each(obj, function (prop) {
      // Nothing to do here, just showing that that is a valid filter to hook
      // into.
    });
  },

  // Gets called on every update before a tween state is calculated.
  'beforeTween': function beforeTween (currentState, fromState, toState) {
    Tweenable.each(toState, function (prop) {
      // Double each target property right before the tween formula is applied.
      obj[prop] *= 2;
    });
  },

  // Gets called on every update after a tween state is calculated.
  'afterTween': function afterTween (currentState, fromState, toState) {
    Tweenable.each(toState, function (prop) {
      // Return the target properties back to their pre-doubled values.
      obj[prop] /= 2;
    });
  }
}
````

Yes, having `doubler` filter is useless.  A more practical use of filters is to
add support for more data types.  __Remember, Shifty only supports `Numbers`
by default__, but you can add support for strings, functions, or whatever else
you might need.  For example, the Token extension works by filtering string
values into numbers before each tween step, and then back again after the tween
step.

## Building Shifty

Shifty uses [nodejs](http://nodejs.org) and [Grunt](http://gruntjs.com/) for
the build system. It also requires a handful of Node modules for the build
process.  Install the dependencies via npm like so:

````
$: npm install
````

Once those are installed, do this at the command line to build the project:

````
$: grunt build
````

The the default `build` task creates a binary that includes all extensions.
You can also create minimal binaries that only include the bare essentials for
Shifty to run:

````
$: grunt build-minimal
````

Note that a minimal build includes no tweening formulas.  You can customize and
add build targets in the `grunt.js` file.  You can also lint the code and run
the unit tests with the default Grunt task:

````
$: grunt
````

To generate the documentation:

````
$: grunt yuidoc
````

## AMD and NodeJS

If an AMD loader (eg. [RequireJS](http://requirejs.org/),
[Curl.js](https://github.com/unscriptable/curl)) is present on the page, Shifty
won't generate any globals, so to use it you must list `"shifty"` as
a dependency.

````javascript
define(['shifty'], function(Tweenable){
  var tweenable = new Tweenable();
});
````

Shifty can also be used in NodeJS:

````javascript
var Tweenable = require('shifty');
````

## Contributors

Take a peek at the [Network](https://github.com/jeremyckahn/shifty/network)
page to see all of the Shifty contributors, but
[@millermedeiros](https://github.com/millermedeiros) in particular deserves
recognition for his patches to make Shifty compatible with Node.

Also, special thanks goes to [Thomas Fuchs](https://twitter.com/thomasfuchs):
Shifty's easing formulas and Bezier curve code was adapted from his awesome
[Scripty2](https://github.com/madrobby/scripty2) project.

## License

Shifty is distributed under the [MIT
license](http://opensource.org/licenses/MIT).  You are encouraged to use and
modify the code to suit your needs, as well as redistribute it.
