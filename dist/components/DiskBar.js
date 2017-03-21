'use strict';

Object.defineProperty(exports, "__esModule", {
	value: true
});

var _systeminformation = require('systeminformation');

var _systeminformation2 = _interopRequireDefault(_systeminformation);

var _progressbar = require('progressbar.js');

var _config = require('../config');

var _helpers = require('../helpers');

var _helpers2 = _interopRequireDefault(_helpers);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

exports.default = {
	template: '<div class="cont fl" id="disk-cont">DISK</div>',
	data: function data() {
		return {
			dUsed: 0,
			dTotal: 0,
			diskBar: null
		};
	},
	mounted: function mounted() {
		var _this = this;

		this.diskBar = new _progressbar.SemiCircle('#disk-cont', {
			strokeWidth: 12,
			color: _config.prop.diskBarColor,
			trailColor: _config.prop.trailColor,
			easing: 'easeInOut',
			duration: _config.prop.diskDuration,
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
				bar.setText(_this.dUsed + ' / ' + _this.dTotal + 'GB');
			}
		});

		this.diskInfo();
		setInterval(this.diskInfo, _config.prop.diskDuration);
	},

	methods: {
		diskInfo: function diskInfo() {
			var _this2 = this;

			_systeminformation2.default.fsSize(function (disk) {
				_this2.dUsed = _helpers2.default.prettyDiskSize(disk[0].used);
				_this2.dTotal = _helpers2.default.prettyDiskSize(disk[0].size);
				_this2.diskBar.animate(disk[0].use / 100);
			});
		}
	}
};