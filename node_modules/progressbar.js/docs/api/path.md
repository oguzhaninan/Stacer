# new Path(path, [*options*])

Custom shaped progress bar. You can create arbitrary shaped progress bars by
passing a SVG path created with e.g. Adobe Illustrator. It's on caller's responsibility to append SVG to DOM.

!!! note

    Remember to add e.g. `stroke="1"` and `fill-opacity="0"` attributes for the SVG path.
    They will reveal the true shape of the path.

**Example**

Assuming there was SVG object with heart shaped path in HTML

```html
<svg xmlns="http://www.w3.org/2000/svg" version="1.1" x="0px" y="0px" viewBox="0 0 100 100">
    <path fill-opacity="0" stroke-width="0.5" stroke="#f4f4f4" d="M81.495,13.923c-11.368-5.261-26.234-0.311-31.489,11.032C44.74,13.612,29.879,8.657,18.511,13.923  C6.402,19.539,0.613,33.883,10.175,50.804c6.792,12.04,18.826,21.111,39.831,37.379c20.993-16.268,33.033-25.344,39.819-37.379  C99.387,33.883,93.598,19.539,81.495,13.923z"/>
    <path id="heart-path" fill-opacity="0" stroke-width="0.6" stroke="#555" d="M81.495,13.923c-11.368-5.261-26.234-0.311-31.489,11.032C44.74,13.612,29.879,8.657,18.511,13.923  C6.402,19.539,0.613,33.883,10.175,50.804c6.792,12.04,18.826,21.111,39.831,37.379c20.993-16.268,33.033-25.344,39.819-37.379  C99.387,33.883,93.598,19.539,81.495,13.923z"/>
</svg>
```

Initialization would be this easy

```javascript
var svgPath = document.getElementById('heart-path');
var path = new ProgressBar.Path(svgPath, {
    duration: 300
});
```

**Working with embedded SVG**

If the SVG was not inline in the HTML but instead in, say,
an `<object>` tag, we'd have to take extra steps to wait until it has loaded
and then access it differently since it's in a separate DOM tree.
Given e.g.:

```html
<object id="heart" type="image/svg+xml" data="heart.svg">No SVG support :(</object>
```

we could do

```javascript
var heart = document.getElementById('heart');
heart.addEventListener('load', function() {
var path = new ProgressBar.Path(heartObject.contentDocument.querySelector('#heart-path'), {
    duration: 300
});
```

**Parameters**

* `path` [SVG Path](https://developer.mozilla.org/en-US/docs/Web/SVG/Tutorial/Paths) object or plain selector string. For example `$('svg > path:first-child')[0]`.
* `options` Animation options.

    ```javascript
    {
        // Duration for animation in milliseconds
        // Default: 800
        duration: 1200,

        // Easing for animation. See #easing section.
        // Default: 'linear'
        easing: 'easeIn',

        // Attachment which can be any object
        // you need to modify within the step function.
        // Passed as a parameter to step function.
        // Default: undefined
        attachment: document.querySelector('#container > svg'),

        // See #custom-animations section
        from: { color: '#eee' },
        to: { color: '#000' },
        step: function(state, path, attachment) {
            // Do any modifications to attachment and/or path attributes
        }
    }
    ```

## .path

Reference to [SVG path](https://developer.mozilla.org/en-US/docs/Web/SVG/Element/path) which presents the actual progress bar.

## .animate(progress, [*options*], [*cb*])

Animates drawing of path.

**Example**

```javascript
path.animate(0.3, {
    duration: 800
}, function() {
    console.log('Animation has finished');
});
```

**Parameters**

* `progress` progress from 0 to 1.
* `options` Animation options. These options override the defaults given in initialization.

    ```javascript
    {
        // Duration for animation in milliseconds
        // Default: 800
        duration: 1200,

        // Easing for animation. See #easing section.
        // Default: 'linear'
        easing: 'easeOut',

        // Attachment which can be any object
        // you need to modify within the step function.
        // Passed as a parameter to step function.
        // Default: undefined
        attachment: document.querySelector('#container > svg'),

        // See #custom-animations section
        from: { color: '#eee' },
        to: { color: '#000' },
        step: function(state, path, attachment) {
            // Do any modifications to attachment and/or path attributes
        }
    }
    ```

* `cb` Callback function which is called after transition ends.

## .set(progress)

Set progress instantly without animation. Clears all transitions
for path.

## .stop()

Stops animation to its current position.

## .value()

Returns current shown progress from 0 to 1. This value changes when animation is running.
