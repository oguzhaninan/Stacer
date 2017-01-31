// Circle shaped progress bar

var Shape = require('./shape');
var utils = require('./utils');

var Circle = function Circle(container, options) {
    // Use two arcs to form a circle
    // See this answer http://stackoverflow.com/a/10477334/1446092
    this._pathTemplate =
        'M 50,50 m 0,-{radius}' +
        ' a {radius},{radius} 0 1 1 0,{2radius}' +
        ' a {radius},{radius} 0 1 1 0,-{2radius}';

    this.containerAspectRatio = 1;

    Shape.apply(this, arguments);
};

Circle.prototype = new Shape();
Circle.prototype.constructor = Circle;

Circle.prototype._pathString = function _pathString(opts) {
    var widthOfWider = opts.strokeWidth;
    if (opts.trailWidth && opts.trailWidth > opts.strokeWidth) {
        widthOfWider = opts.trailWidth;
    }

    var r = 50 - widthOfWider / 2;

    return utils.render(this._pathTemplate, {
        radius: r,
        '2radius': r * 2
    });
};

Circle.prototype._trailString = function _trailString(opts) {
    return this._pathString(opts);
};

module.exports = Circle;
