'use strict';

Object.defineProperty(exports, "__esModule", {
    value: true
});

var _child_process = require('child_process');

var _sudoPrompt = require('sudo-prompt');

var _sudoPrompt2 = _interopRequireDefault(_sudoPrompt);

var _fs = require('fs');

var _fs2 = _interopRequireDefault(_fs);

var _config = require('../config');

var _helpers = require('../helpers');

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

function _toConsumableArray(arr) { if (Array.isArray(arr)) { for (var i = 0, arr2 = Array(arr.length); i < arr.length; i++) { arr2[i] = arr[i]; } return arr2; } else { return Array.from(arr); } }

exports.default = {
    template: '<div>\n\t\t\t\t<div class="cleaner-sidebar">\n\t\t\t\t\t<ul>\n\t\t\t\t\t\t<li><label><input type="checkbox" v-model:checked="aptCacheSelect"><i></i>\n\t\t\t\t\t\t<span> Apt Cache ({{ aptCachesList.length }}) </span></label></li>\n\n\t\t\t\t\t\t<li><label><input type="checkbox" v-model:checked="crashReportsSelect"><i></i>\n\t\t\t\t\t\t<span> Crash Reports ({{ crashReportsList.length }}) </span></label></li>\n\n\t\t\t\t\t\t<li><label><input type="checkbox" v-model:checked="systemLogsSelect"><i></i>\n\t\t\t\t\t\t<span> System Logs ({{ systemLogsList.length }}) </span></label></li>\n\n\t\t\t\t\t\t<li><label><input type="checkbox" v-model:checked="appCacheSelect"><i></i>\n\t\t\t\t\t\t<span> App Cache ({{ appCachesList.length }}) </span></label></li>\n\t\t\t\t\t</ul>\n\n\t\t\t\t\t<input type="button" id="system-scan-btn" @click="systemScan" value="System Scan" />\n\t\t\t\t</div>\n\n\t\t\t\t<div class="tdl-holder scroll" id="cleaner-table">\n\t\t\t\t\t<div class="tdl-content">\n\t\t\t\t\t\t<ul>\n\t\t\t\t\t\t\t<li v-show="aptCachesList.length" style="background-color: #293945;">\n\t\t\t\t\t\t\t\t<label>\n\t\t\t\t\t\t\t\t\t<input type="checkbox" @change="checkAllAptCaches"><i></i>\n\t\t\t\t\t\t\t\t\t<span style="font-size:14px; color:#aeb5bf;">\n\t\t\t\t\t\t\t\t\t\tSelect All (Apt Cache)\n\t\t\t\t\t\t\t\t\t</span>\n\t\t\t\t\t\t\t\t</label>\n\t\t\t\t\t\t\t</li>\n\t\t\t\t\t\t\t<li v-for="cacheName in aptCachesList">\n\t\t\t\t\t\t\t\t<label>\n\t\t\t\t\t\t\t\t\t<input type="checkbox" :value="cacheName" v-model="checkedAptCaches"  >\n\t\t\t\t\t\t\t\t\t<i></i>\n\t\t\t\t\t\t\t\t\t<span> {{ cacheName }} </span>\n\t\t\t\t\t\t\t\t</label>\n\t\t\t\t\t\t\t</li>\n\n\t\t\t\t\t\t\t<li v-show="crashReportsList.length" style="background-color: #293945;">\n\t\t\t\t\t\t\t\t<label>\n\t\t\t\t\t\t\t\t\t<input type="checkbox" @change="checkAllCrashReports">\n\t\t\t\t\t\t\t\t\t<i></i>\n\t\t\t\t\t\t\t\t\t<span style="font-size:14px; color:#aeb5bf;">\n\t\t\t\t\t\t\t\t\t\tSelect All (Crash Reports)\n\t\t\t\t\t\t\t\t\t</span>\n\t\t\t\t\t\t\t\t</label>\n\t\t\t\t\t\t\t</li>\n\t\t\t\t\t\t\t<li v-for="crashName in crashReportsList">\n\t\t\t\t\t\t\t\t<label>\n\t\t\t\t\t\t\t\t\t<input type="checkbox" :value="crashName" v-model="checkedCrashReports">\n\t\t\t\t\t\t\t\t\t<i></i>\n\t\t\t\t\t\t\t\t\t<span> {{ crashName }} </span>\n\t\t\t\t\t\t\t\t</label>\n\t\t\t\t\t\t\t</li>\n\n\t\t\t\t\t\t\t<li v-show="systemLogsList.length" style="background-color: #293945;">\n\t\t\t\t\t\t\t\t<label>\n\t\t\t\t\t\t\t\t\t<input type="checkbox" @change="checkAllSystemLogs"><i></i>\n\t\t\t\t\t\t\t\t\t<span style="font-size:14px; color:#aeb5bf;">\n\t\t\t\t\t\t\t\t\t\tSelect All (System Logs)\n\t\t\t\t\t\t\t\t\t</span>\n\t\t\t\t\t\t\t\t</label>\n\t\t\t\t\t\t\t</li>\n\t\t\t\t\t\t\t<li v-for="logName in systemLogsList">\n\t\t\t\t\t\t\t\t<label>\n\t\t\t\t\t\t\t\t\t<input type="checkbox" :value="logName" v-model="checkedSystemLogs">\n\t\t\t\t\t\t\t\t\t<i></i>\n\t\t\t\t\t\t\t\t\t<span> {{ logName }} </span>\n\t\t\t\t\t\t\t\t</label>\n\t\t\t\t\t\t\t</li>\n\n\t\t\t\t\t\t\t<li v-show="appCachesList.length" style="background-color: #293945;">\n\t\t\t\t\t\t\t\t<label>\n\t\t\t\t\t\t\t\t\t<input type="checkbox" @change="checkAllAppCaches"><i></i>\n\t\t\t\t\t\t\t\t\t<span style="font-size:14px; color:#aeb5bf;">\n\t\t\t\t\t\t\t\t\t\tSelect All (App Caches)\n\t\t\t\t\t\t\t\t\t</span>\n\t\t\t\t\t\t\t\t</label>\n\t\t\t\t\t\t\t</li>\n\t\t\t\t\t\t\t<li v-for="appName in appCachesList">\n\t\t\t\t\t\t\t\t<label>\n\t\t\t\t\t\t\t\t\t<input type="checkbox" :value="appName" v-model="checkedAppCaches">\n\t\t\t\t\t\t\t\t\t<i></i>\n\t\t\t\t\t\t\t\t\t<span>\n\t\t\t\t\t\t\t\t\t\t{{ appName }}\n\t\t\t\t\t\t\t\t\t</span>\n\t\t\t\t\t\t\t\t</label>\n\t\t\t\t\t\t\t</li>\n\t\t\t\t\t\t</ul>\n\t\t\t\t\t</div>\n\t\t\t\t</div>\n\t\t\t\t<input type="button" id="clean-btn" @click="systemClean" value="Clean" />\n\t\t\t</div>',
    data: function data() {
        return {
            aptCacheSelect: false,
            crashReportsSelect: false,
            systemLogsSelect: false,
            appCacheSelect: false,

            aptCachesList: [],
            crashReportsList: [],
            systemLogsList: [],
            appCachesList: [],

            checkedAptCaches: [],
            checkedCrashReports: [],
            checkedSystemLogs: [],
            checkedAppCaches: []
        };
    },

    methods: {
        systemScan: function systemScan() {
            var _this = this;

            this.aptCachesList = [];
            this.crashReportsList = [];
            this.systemLogsList = [];
            this.appCachesList = [];

            if (this.aptCacheSelect) {
                _fs2.default.readdir(_config.commands.aptCachePath, 'utf8', function (err, files) {
                    if (!err) files.filter(function (file) {
                        return file.endsWith('.deb');
                    }).forEach(function (file) {
                        return _this.aptCachesList.push(file);
                    });else console.log(err);
                });
            } else {
                this.checkedAptCaches = [];
            }

            if (this.crashReportsSelect) {
                _fs2.default.readdir(_config.commands.crashReportsPath, 'utf8', function (err, files) {
                    if (!err) files.forEach(function (file) {
                        return _this.crashReportsList.push(file);
                    });else console.log(err);
                });
            } else {
                this.checkedCrashReports = [];
            }

            if (this.systemLogsSelect) {
                _fs2.default.readdir(_config.commands.systemLogsPath, 'utf8', function (err, files) {
                    if (!err) files.forEach(function (file) {
                        return _this.systemLogsList.push(file);
                    });else console.log(err);
                });
            } else {
                this.checkedSystemLogs = [];
            }

            if (this.appCacheSelect) {
                _fs2.default.readdir(_config.commands.appCachePath, 'utf8', function (err, files) {
                    if (!err) files.forEach(function (file) {
                        return _this.appCachesList.push(file);
                    });else console.log(err);
                });
            } else {
                this.checkedAppCaches = [];
            }
        },
        systemClean: function systemClean() {
            var _this2 = this;

            var filesToRemove = '';

            if (this.checkedAptCaches) {
                this.checkedAptCaches.forEach(function (file) {
                    filesToRemove += 'rm -rf ' + _config.commands.aptCachePath + file + '; ';
                });
            }

            if (this.checkedCrashReports) {
                this.checkedCrashReports.forEach(function (file) {
                    filesToRemove += 'rm -rf ' + _config.commands.crashReportsPath + file + '; ';
                });
            }

            if (this.checkedSystemLogs) {
                this.checkedSystemLogs.forEach(function (file) {
                    filesToRemove += 'rm -rf ' + _config.commands.systemLogsPath + file + '; ';
                });
            }

            if (this.checkedAppCaches) {
                this.checkedAppCaches.forEach(function (file) {
                    filesToRemove += 'rm -rf ' + _config.commands.appCachePath + file + '; ';
                });
            }

            if (filesToRemove) {
                _sudoPrompt2.default.exec((0, _helpers.command)(filesToRemove), { name: 'Stacer' }, function (error, stdout, stderr) {
                    if (stderr) (0, _helpers.showMessage)('System cleaning failed.', 'error');else {
                        _this2.aptCachesList = _this2.aptCachesList.filter(function (item) {
                            return _this2.checkedAptCaches.indexOf(item) == -1;
                        });
                        _this2.crashReportsList = _this2.crashReportsList.filter(function (item) {
                            return _this2.checkedCrashReports.indexOf(item) == -1;
                        });
                        _this2.systemLogsList = _this2.systemLogsList.filter(function (item) {
                            return _this2.checkedSystemLogs.indexOf(item) == -1;
                        });
                        _this2.appCachesList = _this2.appCachesList.filter(function (item) {
                            return _this2.checkedAppCaches.indexOf(item) == -1;
                        });

                        _this2.checkedAptCaches = [];
                        _this2.checkedCrashReports = [];
                        _this2.checkedSystemLogs = [];
                        _this2.checkedAppCaches = [];

                        (0, _helpers.showMessage)('System cleaned.', 'success');
                    }
                });
            }
        },
        checkAllAptCaches: function checkAllAptCaches(e) {
            var _checkedAptCaches;

            this.checkedAptCaches = [];
            if (e.target.checked) (_checkedAptCaches = this.checkedAptCaches).push.apply(_checkedAptCaches, _toConsumableArray(this.aptCachesList));
        },
        checkAllCrashReports: function checkAllCrashReports(e) {
            var _checkedCrashReports;

            this.checkedCrashReports = [];
            if (e.target.checked) (_checkedCrashReports = this.checkedCrashReports).push.apply(_checkedCrashReports, _toConsumableArray(this.crashReportsList));
        },
        checkAllSystemLogs: function checkAllSystemLogs(e) {
            var _checkedSystemLogs;

            this.checkedSystemLogs = [];
            if (e.target.checked) (_checkedSystemLogs = this.checkedSystemLogs).push.apply(_checkedSystemLogs, _toConsumableArray(this.systemLogsList));
        },
        checkAllAppCaches: function checkAllAppCaches(e) {
            var _checkedAppCaches;

            this.checkedAppCaches = [];
            if (e.target.checked) (_checkedAppCaches = this.checkedAppCaches).push.apply(_checkedAppCaches, _toConsumableArray(this.appCachesList));
        }
    }
};