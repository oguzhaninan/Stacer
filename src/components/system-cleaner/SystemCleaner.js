import {
    spawn
} from 'child_process'
import sudo from 'sudo-prompt'
import fs from 'fs'
import {
    commands
} from '../../utils/config'
import {
    command,
    showMessage
} from '../../utils/helpers'

export default {
    template: `<div>
				<div class="cleaner-sidebar">
					<ul>
						<li><label><input type="checkbox" v-model:checked="aptCacheSelect"><i></i>
						<span> Apt Cache ({{ aptCachesList.length }}) </span></label></li>

						<li><label><input type="checkbox" v-model:checked="crashReportsSelect"><i></i>
						<span> Crash Reports ({{ crashReportsList.length }}) </span></label></li>

						<li><label><input type="checkbox" v-model:checked="systemLogsSelect"><i></i>
						<span> System Logs ({{ systemLogsList.length }}) </span></label></li>

						<li><label><input type="checkbox" v-model:checked="appCacheSelect"><i></i>
						<span> App Cache ({{ appCachesList.length }}) </span></label></li>
					</ul>

					<input type="button" id="system-scan-btn" @click="systemScan" value="System Scan" />
				</div>

				<div class="tdl-holder scroll" id="cleaner-table">
					<div class="tdl-content">
						<ul>
							<li v-show="aptCachesList.length" style="background-color: #293945;">
								<label>
									<input type="checkbox" @change="checkAllAptCaches"><i></i>
									<span style="font-size:14px; color:#aeb5bf;">
										Select All (Apt Cache)
									</span>
								</label>
							</li>
							<li v-for="cacheName in aptCachesList">
								<label>
									<input type="checkbox" :value="cacheName" v-model="checkedAptCaches"  >
									<i></i>
									<span> {{ cacheName }} </span>
								</label>
							</li>

							<li v-show="crashReportsList.length" style="background-color: #293945;">
								<label>
									<input type="checkbox" @change="checkAllCrashReports">
									<i></i>
									<span style="font-size:14px; color:#aeb5bf;">
										Select All (Crash Reports)
									</span>
								</label>
							</li>
							<li v-for="crashName in crashReportsList">
								<label>
									<input type="checkbox" :value="crashName" v-model="checkedCrashReports">
									<i></i>
									<span> {{ crashName }} </span>
								</label>
							</li>

							<li v-show="systemLogsList.length" style="background-color: #293945;">
								<label>
									<input type="checkbox" @change="checkAllSystemLogs"><i></i>
									<span style="font-size:14px; color:#aeb5bf;">
										Select All (System Logs)
									</span>
								</label>
							</li>
							<li v-for="logName in systemLogsList">
								<label>
									<input type="checkbox" :value="logName" v-model="checkedSystemLogs">
									<i></i>
									<span> {{ logName }} </span>
								</label>
							</li>

							<li v-show="appCachesList.length" style="background-color: #293945;">
								<label>
									<input type="checkbox" @change="checkAllAppCaches"><i></i>
									<span style="font-size:14px; color:#aeb5bf;">
										Select All (App Caches)
									</span>
								</label>
							</li>
							<li v-for="appName in appCachesList">
								<label>
									<input type="checkbox" :value="appName" v-model="checkedAppCaches">
									<i></i>
									<span>
										{{ appName }}
									</span>
								</label>
							</li>
						</ul>
					</div>
				</div>
				<input type="button" id="clean-btn" @click="systemClean" value="Clean" />
			</div>`,
    data() {
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
        }
    },
    methods: {
        systemScan() {
            // Lists cleaning             
            this.aptCachesList = []
            this.crashReportsList = []
            this.systemLogsList = []
            this.appCachesList = []

            if (this.aptCacheSelect) {
                fs.readdir(commands.aptCachePath, 'utf8', (err, files) => {
                    if (!err)
                        files.filter(file => file.endsWith('.deb'))
                        .forEach(file => this.aptCachesList.push(file))
                    else
                        console.log(err)
                })
            } else {
                this.checkedAptCaches = []
            }

            if (this.crashReportsSelect) {
                fs.readdir(commands.crashReportsPath, 'utf8', (err, files) => {
                    if (!err)
                        files.forEach(file => this.crashReportsList.push(file))
                    else
                        console.log(err)
                })
            } else {
                this.checkedCrashReports = []
            }

            if (this.systemLogsSelect) {
                fs.readdir(commands.systemLogsPath, 'utf8', (err, files) => {
                    if (!err)
                        files.forEach(file => this.systemLogsList.push(file))
                    else
                        console.log(err)
                })
            } else {
                this.checkedSystemLogs = []
            }

            if (this.appCacheSelect) {
                fs.readdir(commands.appCachePath, 'utf8', (err, files) => {
                    if (!err)
                        files.forEach(file => this.appCachesList.push(file))
                    else
                        console.log(err)
                })
            } else {
                this.checkedAppCaches = []
            }
        },
        // System clean
        systemClean() {
            let filesToRemove = ''

            if (this.checkedAptCaches) {
                this.checkedAptCaches.forEach(file => {
                    filesToRemove += `rm -rf ${commands.aptCachePath}${file}; `
                })
            }

            if (this.checkedCrashReports) {
                this.checkedCrashReports.forEach(file => {
                    filesToRemove += `rm -rf ${commands.crashReportsPath}${file}; `
                })
            }

            if (this.checkedSystemLogs) {
                this.checkedSystemLogs.forEach(file => {
                    filesToRemove += `rm -rf ${commands.systemLogsPath}${file}; `
                })
            }

            if (this.checkedAppCaches) {
                this.checkedAppCaches.forEach(file => {
                    filesToRemove += `rm -rf ${commands.appCachePath}${file}; `
                })
            }

            if (filesToRemove) {
                sudo.exec(command(filesToRemove), {
                        name: 'Stacer'
                    },
                    (error, stdout, stderr) => {
                        if (stderr)
                            showMessage(`System cleaning failed.`, 'error')
                        else {
                            this.aptCachesList = this.aptCachesList.filter(item => this.checkedAptCaches.indexOf(item) == -1)
                            this.crashReportsList = this.crashReportsList.filter(item => this.checkedCrashReports.indexOf(item) == -1)
                            this.systemLogsList = this.systemLogsList.filter(item => this.checkedSystemLogs.indexOf(item) == -1)
                            this.appCachesList = this.appCachesList.filter(item => this.checkedAppCaches.indexOf(item) == -1)

                            this.checkedAptCaches = []
                            this.checkedCrashReports = []
                            this.checkedSystemLogs = []
                            this.checkedAppCaches = []

                            showMessage(`System cleaned.`, 'success')
                        }
                    })
            }
        },
        // Check all items         
        checkAllAptCaches(e) {
            this.checkedAptCaches = []
            if (e.target.checked)
                this.checkedAptCaches.push(...this.aptCachesList)
        },
        checkAllCrashReports(e) {
            this.checkedCrashReports = []
            if (e.target.checked)
                this.checkedCrashReports.push(...this.crashReportsList)
        },
        checkAllSystemLogs(e) {
            this.checkedSystemLogs = []
            if (e.target.checked)
                this.checkedSystemLogs.push(...this.systemLogsList)
        },
        checkAllAppCaches(e) {
            this.checkedAppCaches = []
            if (e.target.checked)
                this.checkedAppCaches.push(...this.appCachesList)
        }
    },
    watch: {
        appCacheSelect: (checked) => {
            if (checked) {
                showMessage('Deleting applications\' caches can cause problems. Please be careful.', 'warning')
            }
        }
    }
}