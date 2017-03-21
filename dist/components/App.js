'use strict';

Object.defineProperty(exports, "__esModule", {
	value: true
});

var _Dashboard = require('./Dashboard');

var _Dashboard2 = _interopRequireDefault(_Dashboard);

var _StartupApps = require('./StartupApps');

var _StartupApps2 = _interopRequireDefault(_StartupApps);

var _Services = require('./Services');

var _Services2 = _interopRequireDefault(_Services);

var _Uninstaller = require('./Uninstaller');

var _Uninstaller2 = _interopRequireDefault(_Uninstaller);

var _SystemCleaner = require('./SystemCleaner');

var _SystemCleaner2 = _interopRequireDefault(_SystemCleaner);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

exports.default = {
	template: '<div id="main" class="fl w100">\n\n\t\t\t\t\t<ul id="nav">\n\t\t\t\t\t\t<li><label :class="{\'nav-active\': activeNav == 1}" @click="activeNav = 1">Dashboard</label></li>\n\t\t\t\t\t\t<li><label :class="{\'nav-active\': activeNav == 2}" @click="activeNav = 2">System Cleaner</label></li>\n\t\t\t\t\t\t<li><label :class="{\'nav-active\': activeNav == 3}" @click="activeNav = 3">Startup Apps</label></li>\n\t\t\t\t\t\t<li><label :class="{\'nav-active\': activeNav == 4}" @click="activeNav = 4">Services</label></li>\n\t\t\t\t\t\t<li><label :class="{\'nav-active\': activeNav == 5}" @click="activeNav = 5">Uninstaller</label></li>\n\t\t\t\t\t</ul>\n\n\t\t\t\t\t<!--Dashboard Page-->\n\t\t\t\t\t<transition name="slide-fade">\n\t\t\t\t\t\t<div v-show="activeNav == 1" class="content">\n\t\t\t\t\t\t\t<dashboard/>\n\t\t\t\t\t\t</div>\n\t\t\t\t\t</transition>\n\n\t\t\t\t\t<!--System Cleaner Page-->\n\t\t\t\t\t<transition name="slide-fade">\n\t\t\t\t\t\t<div v-show="activeNav == 2" class="content">\n\t\t\t\t\t\t\t<system-cleaner/>\n\t\t\t\t\t\t</div>\n\t\t\t\t\t</transition>\n\n\t\t\t\t\t<!--System Startup Apps Page-->\n\t\t\t\t\t<transition name="slide-fade">\n\t\t\t\t\t\t<div v-show="activeNav == 3" class="content">\t\t\t\t\n\t\t\t\t\t\t\t<startup-apps/>\n\t\t\t\t\t\t</div>\n\t\t\t\t\t</transition>\n\n\t\t\t\t\t<!--Services Page-->\n\t\t\t\t\t<transition name="slide-fade">\n\t\t\t\t\t\t<div v-show="activeNav == 4" class="content">\n\t\t\t\t\t\t\t<services/>\n\t\t\t\t\t\t</div>\n\t\t\t\t\t</transition>\n\n\t\t\t\t\t<!--Packages Page-->\n\t\t\t\t\t<transition name="slide-fade">\n\t\t\t\t\t\t<div v-show="activeNav == 5" class="content">\n\t\t\t\t\t\t\t<uninstaller/>\n\t\t\t\t\t\t</div>\n\t\t\t\t\t</transition>\n\n\t\t\t\t\t<!--Startup Loading-->\n\t\t\t\t\t<div id="loading" >\n\t\t\t\t\t\t<span class="loader_text"> Loading... </span>\n\n\t\t\t\t\t\t<div id="loader">\n\t\t\t\t\t\t\t<span></span><span></span><span></span>\n\t\t\t\t\t\t\t<span></span><span></span><span></span>\n\t\t\t\t\t\t\t<span></span><span></span><span></span>\n\t\t\t\t\t\t\t<span></span><span></span><span></span>\n\t\t\t\t\t\t\t<span></span><span></span><span></span>\n\t\t\t\t\t\t\t<span></span><span></span><span></span>\n\t\t\t\t\t\t\t<span></span><span></span>\n\t\t\t\t\t\t</div>\n\n\t\t\t\t\t</div>\n\n\t\t\t\t</div>',
	data: function data() {
		return { activeNav: 1 };
	},

	components: {
		'dashboard': _Dashboard2.default,
		'system-cleaner': _SystemCleaner2.default,
		'startup-apps': _StartupApps2.default,
		'services': _Services2.default,
		'uninstaller': _Uninstaller2.default
	}
};