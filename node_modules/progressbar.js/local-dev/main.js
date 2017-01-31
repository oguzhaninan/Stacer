var ProgressBar = require('../src/main.js');


function onLoad() {
    var bar = new ProgressBar.Circle('#progress', {
        warnings: true,
        duration: 4000,
        from: {
            color: '#f00'
        },
        to: {
            color: '#0f0'
        },
        step: function(state, bar) {
            bar.path.setAttribute('stroke', state.color);
        },
    });

    bar.animate(1);

    // Expose the bar to global so it is easy to test from console
    window.bar = bar;
}

window.onload = onLoad;
