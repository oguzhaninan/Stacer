'use strict';

Object.defineProperty(exports, "__esModule", {
	value: true
});

var _systeminformation = require('systeminformation');

var _systeminformation2 = _interopRequireDefault(_systeminformation);

var _progressbar = require('progressbar.js');

var _config = require('../config');

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

exports.default = {
	template: '<div class="cont fl" id="cpu-cont">CPU</div>',
	mounted: function mounted() {
		var cpuBar = new _progressbar.SemiCircle('#cpu-cont', {
			strokeWidth: 12,
			color: _config.prop.cpuBarColor,
			trailColor: _config.prop.trailColor,
			easing: 'easeInOut',
			duration: _config.prop.cpuDuration,
			svgStyle: {
				marginTop: '10px'
			},
			text: {
				style: {
					color: '#fff',
					paddingBottom: '20px',
					position: 'absolute',
					left: '50%'
				}
			},
			step: function step(state, bar) {
				bar.setText(Math.round(bar.value() * 100) + "%");
			}
		});

		setInterval(function () {
			_systeminformation2.default.currentLoad(function (val) {
				cpuBar.animate(val.currentload / 100);
			});
		}, _config.prop.cpuDuration);
	}
};