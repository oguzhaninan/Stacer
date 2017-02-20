
import { spawn } from 'child_process'
import sudo from 'sudo-prompt'
import fs from 'fs'
import { commands } from './config'
import { command, showMessage } from './helpers'

Vue.component('system-cleaner', {
    template: '#system-cleaner',
    data () {
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
            /**
             * Lists cleaning
             */
            this.aptCachesList = []
            this.crashReportsList = []
            this.systemLogsList = []
            this.appCachesList = []
            
            if ( this.aptCacheSelect ) {
                fs.readdir( commands.aptCachePath , 'utf8', (err, files) => {
                    if ( ! err )
                        files.filter( ( file ) => file.endsWith('.deb') )
                            .forEach( ( file ) => this.aptCachesList.push( file ) )
                    else
                        console.log(err)
                })
            }
            else{
                this.checkedAptCaches = []
            }
            
            if( this.crashReportsSelect ) {
                fs.readdir( commands.crashReportsPath , 'utf8', (err, files) => {
                    if ( ! err )
                        files.forEach( ( file ) => this.crashReportsList.push( file ) )                    
                    else                    
                        console.log(err)                    
                })
            }
            else{
                this.checkedCrashReports = []
            }

            if( this.systemLogsSelect ) {
                fs.readdir( commands.systemLogsPath , 'utf8', (err, files) => {                    
                    if ( ! err )
                        files.forEach( ( file ) => this.systemLogsList.push( file ) )
                    else
                        console.log(err)
                })
            }
            else{
                this.checkedSystemLogs = []
            }

            if( this.appCacheSelect ) {
                fs.readdir( commands.appCachePath , 'utf8', (err, files) => {
                    if ( ! err )
                        files.forEach( ( file ) => this.appCachesList.push( file ) )
                    else
                        console.log(err)                    
                })
            }
            else{
                this.checkedAppCaches = []
            }
        },
        /**
         * System clean
         */
        systemClean() {
            let filesToRemove = ''

            if ( this.checkedAptCaches ) {
                this.checkedAptCaches.forEach( ( file ) => {
                    filesToRemove += `rm -rf ${commands.aptCachePath}${file}; `
                })
            }

            if( this.checkedCrashReports ) {
                this.checkedCrashReports.forEach( ( file ) => {
                    filesToRemove += `rm -rf ${commands.crashReportsPath}${file}; `
                })
            }

            if( this.checkedSystemLogs ) {
                this.checkedSystemLogs.forEach( ( file ) => {
                    filesToRemove += `rm -rf ${commands.systemLogsPath}${file}; `
                })
            }

            if( this.checkedAppCaches ) {
                this.checkedAppCaches.forEach( ( file ) => {
                    filesToRemove += `rm -rf ${commands.appCachePath}${file}; `
                })
            }

            if (filesToRemove){
                sudo.exec( command(filesToRemove) , {name: 'Stacer'},
                    (error, stdout, stderr) => {
                        if (stderr)
                            showMessage( `System cleaning failed.` , 'error')
                        else {
                            this.aptCachesList = this.aptCachesList.filter( ( item ) => this.checkedAptCaches.indexOf(item) == -1 )
                            this.crashReportsList = this.crashReportsList.filter( ( item ) => this.checkedCrashReports.indexOf(item) == -1 )
                            this.systemLogsList = this.systemLogsList.filter( ( item ) => this.checkedSystemLogs.indexOf(item) == -1 )
                            this.appCachesList = this.appCachesList.filter( ( item ) => this.checkedAppCaches.indexOf(item) == -1 )

                            this.checkedAptCaches = []
                            this.checkedCrashReports = []
                            this.checkedSystemLogs = []
                            this.checkedAppCaches = []

                            showMessage( `System cleaned.` , 'success')
                        }
                })
            }
        },
        /**
         * Check all items
         */
        checkAllAptCaches( e ) {
            this.checkedAptCaches = []
            if( e.target.checked )
                this.checkedAptCaches.push(...this.aptCachesList)
        },
        checkAllCrashReports( e ) {
            this.checkedCrashReports = []
            if( e.target.checked )
                this.checkedCrashReports.push(...this.crashReportsList)
        },
        checkAllSystemLogs( e ) {
            this.checkedSystemLogs = []
            if ( e.target.checked )
                this.checkedSystemLogs.push(...this.systemLogsList)
        },
        checkAllAppCaches( e ) {
            this.checkedAppCaches = []
            if( e.target.checked )
                this.checkedAppCaches.push(...this.appCachesList)
        }
    }
})
