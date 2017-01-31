// Tests which test shared behaviour of all progress bar shapes

var chai = require('chai');
var chaiStats = require('chai-stats');
chai.use(chaiStats);
var expect = chai.expect;

var PRECISION = 2;
var TEXT_CLASS_NAME = '.progressbar-text';

var sharedTests = function sharedTests() {
    // Test that public attributes exist
    it('.svg attribute should exist', function() {
        expect(this.bar.svg).to.be.ok();
    });

    it('.path attribute should exist', function() {
        expect(this.bar.path).to.be.ok();
    });

    it('.trail attribute should exist', function() {
        expect(this.bar.trail).to.be.ok();
    });

    it('.text attribute should exist', function() {
        expect(this.bar.text).to.be.ok();
    });

    it('bar should be empty after initialization', function() {
        expect(this.bar.value()).to.almost.equal(0, PRECISION);
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

    it('step function should recieve a reference to ProgressBar as argument #2', function() {
        this.bar.animate(1, {duration: 600});
        var allCallsHaveBar = true;

        for (var i = 0; i < this.step.args.length; i++) {
            if (this.step.args[i][1] !== this.bar) {
                allCallsHaveBar = false;
            }
        }

        expect(allCallsHaveBar).to.be.true();
    });

    it('step function should recieve a reference to attachment as argument #3', function() {
        this.bar.animate(1, {duration: 600});
        var allCallsHaveAttachment = true;

        for (var i = 0; i < this.step.args.length; i++) {
            if (this.step.args[i][2] !== this.attachment) {
                allCallsHaveAttachment = false;
            }
        }

        expect(allCallsHaveAttachment).to.be.true();
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

    it('destroy() should delete DOM elements', function() {
        var svg = document.querySelector('svg');
        expect(svg).not.to.equal(null);

        var textElement = document.querySelector(TEXT_CLASS_NAME);
        expect(textElement).not.to.equal(null);

        this.bar.destroy();
        svg = document.querySelector('svg');
        expect(svg).to.equal(null);

        textElement = document.querySelector(TEXT_CLASS_NAME);
        expect(textElement).to.equal(null);
    });

    it('destroy() should make object unusable', function() {
        this.bar.destroy();

        var self = this;
        var methodsShouldThrow = ['destroy', 'value', 'set', 'animate', 'stop', 'setText'];
        methodsShouldThrow.forEach(function(methodName) {
            expect(function shouldThrow() {
                self[methodName]();
            }).to.throw(Error);
        });

        expect(this.bar.svg).to.equal(null);
        expect(this.bar.path).to.equal(null);
        expect(this.bar.trail).to.equal(null);
        expect(this.bar.text).to.equal(null);
    });

    it('setText() should change text element', function() {
        var textElement = document.querySelector(TEXT_CLASS_NAME);
        expect(textElement.textContent).to.equal('Test');
        this.bar.setText('new');

        textElement = document.querySelector(TEXT_CLASS_NAME);
        expect(textElement.textContent).to.equal('new');
    });
};

module.exports = sharedTests;
