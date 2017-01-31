var chai = require('chai');
var chaiStats = require('chai-stats');
chai.use(chaiStats);
var expect = chai.expect;

var PRECISION = 2;
var ANIM_PROP = {
    'styleName': 'stroke-offset',
    'scriptName': 'strokeOffset'
};

var barOpts = {
    duration: 800,
    from: {strokeOffset: 0},
    to: { strokeOffset: 0 },
    step: function(state, self, attachment) {
        attachment.setAttribute(ANIM_PROP.scriptName, state[ANIM_PROP.scriptName]);
    }
};

function createPath() {
    var container = document.querySelector('body'),
        svg = document.createElementNS('http://www.w3.org/2000/svg', 'svg'),
        path = document.createElementNS('http://www.w3.org/2000/svg', 'path');

    svg.setAttribute('version', '1.1');
    svg.setAttribute('id', 'progress-bar');
    svg.setAttribute('x', '0px');
    svg.setAttribute('y', '0px');
    svg.setAttribute('viewBox', '0 0 197 165.39555');
    svg.setAttribute('enable-background', '0 0 197 165.39555');

    var attrs = {
        'id': 'progress-path',
        'fill': 'none',
        'stroke': '#ccc',
        'stroke-width':'15',
        'stroke-miterlimit': '10',
        'd': 'm 31.7,160.3 c -15,-16.2 -24.2,-38 -24.2,-61.8 0,-50.3' +
             ' 40.7,-91 91,-91 50.3,0 91,40.7 91,91 0,23.9 -9.2,45.6 -24.2,61.8'
    };

    for (var i in attrs) {
        path.setAttribute(i, attrs[i]);
    }

    svg.appendChild(path);

    container.appendChild(svg);

    return {
        path: path,
        svg: svg
    };
}

function pathTests() {

    it('step function should recieve a reference to ProgressBar as argument #2', function() {
        this.bar.animate(1, {duration: 500});

        // we only care about the second arg, for each call so we need to manually
        // inspect them since we dont know what state would look like
        var ok = true;

        for (var i = 0; i < this.step.args.length; i++) {
            if (this.step.args[i][1] !== this.bar) {
                ok = false;
            }
        }

        expect(ok).to.be.true();
    });

    it('step function should recieve a reference to ProgressBar as argument #3', function() {
        this.bar.animate(1, {duration: 500});

        // we only care about the third arg, for each call so we need to manually
        // inspect them since we dont know what state would look like
        var ok = true;

        for (var i = 0; i < this.step.args.length; i++) {
            if (this.step.args[i][2] !== this.attachment) {
                ok = false;
            }
        }

        expect(ok).to.be.true();
    });

    it('set should change value', function() {
        this.bar.set(1);
        expect(this.bar.value()).to.almost.equal(1, PRECISION);
    });

    it('animate should change SVG path stroke-dashoffset property', function(done) {
        var progressAtStart = this.bar.value();
        this.bar.animate(1, {duration: 1000});

        var self = this;
        setTimeout(function checkOffsetHasChanged() {
            expect(self.bar.value()).to.be.greaterThan(progressAtStart);
            expect(self.bar.value()).to.be.lessThan(1);
            done();
        }, 100);
    });

    it('animate should change value', function(done) {
        this.bar.set(1);
        this.bar.animate(0, {duration: 600});

        var self = this;
        setTimeout(function checkValueHasChanged() {
            expect(self.bar.value()).not.to.almost.equal(1, PRECISION);
        }, 300);

        setTimeout(function checkAnimationHasCompleted() {
            expect(self.bar.value()).to.almost.equal(0, PRECISION);
            done();
        }, 1200);
    });

    it('stop() should stop animation', function(done) {
        this.bar.animate(1, {duration: 1000});

        var self = this;
        var progressAfterStop;
        setTimeout(function stopAnimation() {
            self.bar.stop();
            progressAfterStop = self.bar.value();
        }, 100);

        setTimeout(function checkProgressAfterStop() {
            expect(progressAfterStop).to.almost.equal(self.bar.value(), PRECISION);
            done();
        }, 400);
    });

    it('.path attribute should exist', function() {
        expect(this.bar.path).to.be.ok();
    });
}

module.exports = {
    options: barOpts,
    runTests: pathTests,
    createPath: createPath
};
