'use strict';

Object.defineProperty(exports, "__esModule", {
	value: true
});

var _systeminformation = require('systeminformation');

var _systeminformation2 = _interopRequireDefault(_systeminformation);

var _progressbar = require('progressbar.js');

var _config = require('../utils/config');

var _helpers = require('../utils/helpers');

var _helpers2 = _interopRequireDefault(_helpers);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

exports.default = {
	template: '<div class="cont fl" id="mem-cont">MEMORY</div>',
	data: function data() {
		return {
			memInfo: 0
		};
	},
	mounted: function mounted() {
		var _this = this;

		var memBar = new _progressbar.SemiCircle('#mem-cont', {
			strokeWidth: 12,
			color: _config.prop.memBarColor,
			trailColor: _config.prop.trailColor,
			easing: 'easeInOut',
			duration: _config.prop.memDuration,
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
				bar.setText(_this.memInfo);
			}
		});

		setInterval(function () {
			_systeminformation2.default.mem(function (ram) {
				var usedMem = ram.total - ram.available;
				var totalMem = ram.total;
				_this.memInfo = _helpers2.default.prettyMemSize(usedMem) + ' / ' + _helpers2.default.prettyMemSize(totalMem) + 'GB';
				memBar.animate(usedMem / totalMem);
			});
		}, _config.prop.memDuration);
	}
};