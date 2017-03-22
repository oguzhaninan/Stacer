'use strict';

Object.defineProperty(exports, "__esModule", {
	value: true
});

var _config = require('../utils/config');

var _helpers = require('../utils/helpers');

var _chokidar = require('chokidar');

var _chokidar2 = _interopRequireDefault(_chokidar);

var _fs = require('fs');

var _fs2 = _interopRequireDefault(_fs);

var _propertiesReader = require('properties-reader');

var _propertiesReader2 = _interopRequireDefault(_propertiesReader);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

exports.default = {
	template: '<div id="startup-apps-table">\n\t\t\t\t\t<div id="startup-apps-title">\n\t\t\t\t\t\t<span style="padding:0;">System Startup Applications ({{apps.length}})</span>\n\t\t\t\t\t</div>\n\t\t\t\t\t<div class="tdl-content scroll">\n\t\t\t\t\t\t<span class="fl w100 empty-list" v-show="! apps.length">\n\t\t\t\t\t\t\tNo startup apps found.\n\t\t\t\t\t\t</span>\n\t\t\t\t\t\t<ul v-show="apps.length">\n\t\t\t\t\t\t\t<li v-for="app in apps">\n\t\t\t\t\t\t\t\t<span>{{ app.name }}</span>\n\t\t\t\t\t\t\t\t<input type="checkbox" class="switch" :id="app.file" :checked="app.isStart" @change="statusChange" />\n\t\t\t\t\t\t\t\t<label :for="app.file"></label>\n\t\t\t\t\t\t\t\t<button :name="app.file" @click="removeApp" class="remove-startup-app"></button>\n\t\t\t\t\t\t\t</li>\n\t\t\t\t\t\t</ul>\n\t\t\t\t\t</div>\n\t\t\t\t\t<button @click="showPrompt = true" class="add-startup-app">Add Startup App</button>\n\t\t\t\t\t\n\t\t\t\t\t<div class="promptDialog" v-show="showPrompt">\n\t\t\t\t\t\t<div class="dialog">\n\t\t\t\t\t\t\t<div>\n\t\t\t\t\t\t\t\t<span>Application</span>\n\t\t\t\t\t\t\t\t<input type="text" v-model="appName" placeholder="App Name" />\n\t\t\t\t\t\t\t\t<input type="text" v-model="appComment" placeholder="App Comment" />\n\t\t\t\t\t\t\t\t<input type="text" v-model="appExec" placeholder="Command" />\n\t\t\t\t\t\t\t\t<button @click="saveApp">Add</button>\n\t\t\t\t\t\t\t\t<button @click="cancelPrompt">Cancel</button>\n\t\t\t\t\t\t\t</div>\n\t\t\t\t\t\t</div>\n\t\t\t\t\t</div>\n\t\t\t\t</div>',
	data: function data() {
		return {
			apps: [],
			showPrompt: false,
			appName: '',
			appComment: '',
			appExec: ''
		};
	},
	created: function created() {
		var _this = this;

		this.getApps();

		_chokidar2.default.watch(_config.commands.autostartApps, {
			persistent: true,
			ignoreInitial: true
		}).on('add', function (path) {
			return _this.getApps();
		}).on('unlink', function (path) {
			return _this.getApps();
		});
	},

	methods: {
		removeApp: function removeApp(e) {
			_fs2.default.unlinkSync(_config.commands.autostartApps + e.target.name);
		},
		saveApp: function saveApp() {
			if (this.appName && this.appComment && this.appExec) {
				var desktopFile = '[Desktop Entry]\n\t\t\t\t\t\t\t\t\t\rName=' + this.appName + '\n\t\t\t\t\t\t\t\t\t\rExec=' + this.appExec + '\n\t\t\t\t\t\t\t\t\t\rComment=' + this.appComment + '\n\t\t\t\t\t\t\t\t\t\rType=ApplicationR\n\t\t\t\t\t\t\t\t\t\rTerminal=false\n\t\t\t\t\t\t\t\t\t\rX-GNOME-Autostart-enabled=true';

				try {
					_fs2.default.writeFileSync(_config.commands.autostartApps + this.appName + '.desktop', desktopFile);
				} catch (err) {} finally {
					this.cancelPrompt();
				}
			} else {
				(0, _helpers.showMessage)('Do not leave required fields blank.', 'error');
			}
		},
		cancelPrompt: function cancelPrompt() {
			this.showPrompt = false;
			this.appName = this.appComment = this.appExec = '';
		},
		getApps: function getApps() {
			var _this2 = this;

			try {
				_fs2.default.readdir(_config.commands.autostartApps, function (err, files) {
					if (!err) {
						_this2.apps.splice(0, _this2.apps.length);
						files.filter(function (file) {
							return file.endsWith('.desktop');
						}).forEach(function (file) {
							try {
								var entry = (0, _propertiesReader2.default)(_config.commands.autostartApps + '/' + file);

								if (entry.get('Desktop Entry.Name') != null) {
									var appName = entry.get('Desktop Entry.Name');
									var isStart = entry.get('Desktop Entry.X-GNOME-Autostart-enabled');

									if (appName != null) {
										_this2.apps.push({
											name: appName,
											file: file,
											isStart: isStart != null ? isStart : true
										});
									}
								}
							} catch (err) {}
						});
					}
				});
			} catch (error) {
				console.log(error);
			}
		},
		statusChange: function statusChange(e) {
			var fileName = e.target.id;
			var isStart = e.target.checked;

			try {
				var data = _fs2.default.readFileSync(_config.commands.autostartApps + '/' + fileName).toString();
				var check = data.match(/\X-GNOME-Autostart-enabled=.*/g);

				if (check) {
					data = data.replace(/\X-GNOME-Autostart-enabled=.*/gi, 'X-GNOME-Autostart-enabled=' + isStart);
				} else {
					data += 'X-GNOME-Autostart-enabled=' + isStart + '\n';
				}

				_fs2.default.writeFileSync(_config.commands.autostartApps + '/' + fileName, data);
			} catch (err) {
				console.log(err);
				(0, _helpers.showMessage)('Operation failed.', 'error');
			}
		}
	}

};