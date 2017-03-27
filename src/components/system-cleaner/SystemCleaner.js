import {
    spawn
} from 'child_process'
import {
    commands
} from '../../utils/config'
import {
    command,
    showMessage
} from '../../utils/helpers'
import fs from 'fs'
import sudo from 'sudo-prompt'
// Components
import SidebarItem from './SidebarItem'
import TableTitle from './TableTitle'
import TableItems from './TableItems'

export default {
    template: `<div>
				<div class="cleaner-side">
					<ul>
                        <sidebar-item text="Apt Cache" :length="aptCachesList.length">
						    <input type="checkbox" v-model:checked="aptCacheSelect">
                        </sidebar-item>

                        <sidebar-item text="Crash Reports" :length="crashReportsList.length">
						    <input type="checkbox" v-model:checked="crashReportsSelect">
                        </sidebar-item>

                        <sidebar-item text="System Logs" :length="systemLogsList.length">
						    <input type="checkbox" v-model:checked="systemLogsSelect">
                        </sidebar-item>

                        <sidebar-item text="App Cache" :length="appCachesList.length">
						    <input type="checkbox" v-model:checked="appCacheSelect">
                        </sidebar-item>

                        <sidebar-item text="Trash" :length="trashSize">
						    <input type="checkbox" v-model:checked="trashSelect">
                        </sidebar-item>
					</ul>

					<input type="button" id="system-scan-btn" @click="systemScan" value="System Scan" />
				</div>

                <div class="item-table scroll">
                    <ul>
                        <table-title text="Apt Caches" :length="aptCachesList.length">
                            <input type="checkbox" @change="checkAllAptCaches">
                        </table-title>
                        
                        <table-items v-for="item in aptCachesList" :text="item">
                            <input type="checkbox" :value="item" v-model="checkedAptCaches">
                        </table-items>


                        <table-title text="Crash Reports" :length="crashReportsList.length">
                            <input type="checkbox" @change="checkAllCrashReports">
                        </table-title>
                        
                        <table-items v-for="item in crashReportsList" :text="item">
                            <input type="checkbox" :value="item" v-model="checkedCrashReports">
                        </table-items>


                        <table-title text="System Logs" :length="systemLogsList.length">
                            <input type="checkbox" @change="checkAllSystemLogs">
                        </table-title>
                        
                        <table-items v-for="item in systemLogsList" :text="item">
                            <input type="checkbox" :value="item" v-model="checkedSystemLogs">
                        </table-items>

                        
                        <table-title text="App Caches" :length="appCachesList.length">
                            <input type="checkbox" @change="checkAllAppCaches">
                        </table-title>
                        
                        <table-items v-for="item in appCachesList" :text="item">
                            <input type="checkbox" :value="item" v-model="checkedAppCaches">
                        </table-items>
                    </ul>
                </div>
				
				<input type="button" id="clean-btn" @click="systemClean" value="Clean" />
			</div>`,

    components: {
        'sidebar-item': SidebarItem,
        'table-title': TableTitle,
        'table-items': TableItems
    },
    data() {
        return {
            aptCacheSelect: false,
            crashReportsSelect: false,
            systemLogsSelect: false,
            appCacheSelect: false,
            trashSelect: false,

            aptCachesList: [],
            crashReportsList: [],
            systemLogsList: [],
            appCachesList: [],

            checkedAptCaches: [],
            checkedCrashReports: [],
            checkedSystemLogs: [],
            checkedAppCaches: [],

            trashSize: '0 Bytes'
        }
    },
    methods: {
        systemScan() {
            // Lists cleaning             
            this.aptCachesList = []
            this.crashReportsList = []
            this.systemLogsList = []
            this.appCachesList = []
            this.trashSize = '0 Bytes'

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

            if (this.trashSelect) {
                function formatBytes(bytes, decimals) {
                    if (bytes == 0) return '0 Bytes'
                    let k = 1000,
                        dm = decimals + 1 || 3,
                        sizes = ['Bytes', 'KB', 'MB', 'GB', 'TB', 'PB', 'EB', 'ZB', 'YB'],
                        i = Math.floor(Math.log(bytes) / Math.log(k))
                    return parseFloat((bytes / Math.pow(k, i)).toFixed(dm)) + ' ' + sizes[i]
                }

                function getSize(path = '') {
                    path += '/'
                    let totalSize = 0
                    try {
                        let trashFiles = fs.readdirSync(commands.trashPath + path)
                        trashFiles.forEach(fileName => {

                            let stats = fs.statSync(commands.trashPath + path + fileName)
                            if (!stats.isDirectory())
                                totalSize += stats.size
                            else
                                totalSize += getSize(path + fileName)
                        })
                    } catch (error) {
                        console.log(error)
                    }
                    return totalSize
                }

                if (fs.existsSync(commands.trashPath))
                    this.trashSize = formatBytes(getSize(), 0)
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

            if (this.trashSelect) {
                filesToRemove += `rm -rf ${commands.trashPath};`
                filesToRemove += `rm -rf ${commands.trashInfoPath};`
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
                            this.trashSize = '0 Bytes'

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
        appCacheSelect: checked => {
            if (checked)
                showMessage('Deleting applications\' caches can cause problems. Please be careful.', 'warning')
        }
    }
}