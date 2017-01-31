Functions use node-style callback convention. Callback function is always the last given parameter.

Shapes have different SVG canvas sizes:

Shape      | Canvas size
-----------|------------------------
Circle     | `100x100`
SemiCircle | `100x50`
Line       | `100x{opts.strokeWidth}`

All shapes are fitted exactly to their canvases.

**Important:** make sure that your container has same aspect ratio
as the SVG canvas. For example: if you are using SemiCircle,
set e.g.

```css
#container {
    width: 300px;
    height: 150px;
}
```

## HTML structure

As an example, we'll use these options:

```js
var circle = new ProgressBar.Circle('#example-percent-container', {
    color: '#FCB03C',
    strokeWidth: 3,
    trailWidth: 1,
    text: {
        value: '0'
    }
});
```

They would produce the following HTML.

```html
<!-- If text is set, position: relative will be applied for the container -->
<div id="container" style="position: relative;">

    <!-- The actual progress bar SVG -->
    <svg viewBox="0 0 100 100">
        <!-- Trail path -->
        <path d="M 50,50 m 0,-48.5 a 48.5,48.5 0 1 1 0,97 a 48.5,48.5 0 1 1 0,-97" stroke="#eee" stroke-width="1" fill-opacity="0"></path>

        <!-- Actual progress bar path -->
        <path d="M 50,50 m 0,-48.5 a 48.5,48.5 0 1 1 0,97 a 48.5,48.5 0 1 1 0,-97" stroke="#FCB03C" stroke-width="3" fill-opacity="0" style="stroke-dasharray: 304.844360351563px, 304.844360351563px; stroke-dashoffset: 304.844360351563px;"></path>
    </svg>

    <!-- Text element created when text option is set -->
    <p class="progressbar-text" style="position: absolute; top: 50%; left: 50%; padding: 0px; margin: 0px; -webkit-transform: translate(-50%, -50%); transform: translate(-50%, -50%); color: rgb(252, 176, 60);">0</p>
</div>
```
