'use strict';

Object.defineProperty(exports, "__esModule", {
	value: true
});

var _systeminformation = require('systeminformation');

var _systeminformation2 = _interopRequireDefault(_systeminformation);

var _progressbar = require('progressbar.js');

var _child_process = require('child_process');

var _config = require('../config');

var _helpers = require('../helpers');

var _helpers2 = _interopRequireDefault(_helpers);

var _SystemInfo = require('./SystemInfo');

var _SystemInfo2 = _interopRequireDefault(_SystemInfo);

var _UpBar = require('./UpBar');

var _UpBar2 = _interopRequireDefault(_UpBar);

var _DownBar = require('./DownBar');

var _DownBar2 = _interopRequireDefault(_DownBar);

var _CpuBar = require('./CpuBar');

var _CpuBar2 = _interopRequireDefault(_CpuBar);

var _MemoryBar = require('./MemoryBar');

var _MemoryBar2 = _interopRequireDefault(_MemoryBar);

var _DiskBar = require('./DiskBar');

var _DiskBar2 = _interopRequireDefault(_DiskBar);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

exports.default = {
	template: '<div>\n\t\t\t\t\t<cpu-bar/>\n\t\t\t\t\t<memory-bar/>\n\t\t\t\t\t<disk-bar/>\n\n\t\t\t\t\t<down-bar/>\n\t\t\t\t\t<system-info/>\n\t\t\t\t\t<up-bar/>\n\n\t\t\t\t\t<!--Update Check-->\n\t\t\t\t\t<div class="fl w100 update-check" v-show="update_check">\n\t\t\t\t\t\t<span>There are updates currently available.</span>\n\t\t\t\t\t\t<button @click="download_update">\n\t\t\t\t\t\t\tDownload Update\n\t\t\t\t\t\t</button>\n\t\t\t\t\t</div>\n\t\t\t\t</div>',
	data: function data() {
		return {
			update_check: false
		};
	},

	components: {
		'system-info': _SystemInfo2.default,
		'up-bar': _UpBar2.default,
		'down-bar': _DownBar2.default,
		'cpu-bar': _CpuBar2.default,
		'memory-bar': _MemoryBar2.default,
		'disk-bar': _DiskBar2.default
	},
	methods: {
		download_update: function download_update() {
			_child_process.shell.openExternal('https://github.com/oguzhaninan/Stacer/releases/latest');
		}
	},
	created: function created() {
		var _this = this;

		try {
			$.getJSON('https://api.github.com/repos/oguzhaninan/Stacer/releases/latest', function (data) {
				var currentVersion = require('../package.json').version.toString();
				var releaseVersion = data.tag_name.substr(1).toString();

				_this.update_check = currentVersion != releaseVersion;
			});
		} catch (error) {
			console.log(error);
		}
	}
};