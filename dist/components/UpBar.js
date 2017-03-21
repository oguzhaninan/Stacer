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
	template: '<div class="line-cont fr">\n\t\t\t\t\t<span>UPLOAD</span>\n\t\t\t\t\t<div id="up-bar"></div>\n\t\t\t\t</div>',
	data: function data() {
		return { upSpeed: 0 };
	},
	mounted: function mounted() {
		var _this = this;

		var upBar = new _progressbar.Line('#up-bar', {
			strokeWidth: 5,
			easing: 'easeInOut',
			duration: _config.prop.networkBarsDuration,
			color: _config.prop.netwrokBarColor,
			trailColor: _config.prop.trailColor,
			text: {
				style: {
					color: '#999',
					position: 'absolute',
					right: '0',
					marginTop: '5px'
				}
			},
			step: function step(state, bar) {
				bar.setText(Math.abs(_this.upSpeed).toString() + ' kB/s');
			}
		});
		_systeminformation2.default.networkInterfaceDefault(function (defaultNetwork) {
			setInterval(function () {
				_systeminformation2.default.networkStats(defaultNetwork, function (data) {
					_this.upSpeed = (data.tx_sec / 1024).toFixed(2);
					upBar.animate(_this.upSpeed / 2000);
				});
			}, _config.prop.networkBarsDuration);
		});
	}
};