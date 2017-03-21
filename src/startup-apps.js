
import { commands } from './config'
import { showMessage } from './helpers'
import chokidar from 'chokidar'
import fs from 'fs'
import properties from 'properties-reader'

let apps = []

function getApps() {
    try {
        fs.readdir( commands.autostartApps, ( err, files ) => {
            if( ! err ) {
                apps.splice(0, apps.length) // array clear
                files.filter( file => file.endsWith('.desktop') )
                     .forEach( file =>
                {
                    try {
                        var entry = properties(commands.autostartApps+ '/' + file)

                        if ( entry.get('Desktop Entry.Name') != null )
                        {
                            let appName = entry.get('Desktop Entry.Name')
                            let isStart = entry.get('Desktop Entry.X-GNOME-Autostart-enabled')

                            if ( appName != null ){
                                apps.push( {
                                    name: appName,
                                    file: file,
                                    isStart: ( isStart != null ? isStart: true )
                                })
                            }
                        }
                    }
                    catch(err){}
                })
            }
        })
    }
    catch (error) {
        console.log( error )
    }
}

/**
 * Watch autostart files
 */
chokidar.watch(commands.autostartApps, { persistent: true, ignoreInitial: true })
    .on('add', path => { getApps() })
    .on('unlink', path => { getApps() })

getApps()

Vue.component('startup-apps', {
    template: '#startup-apps',
    data() {
        return { apps: apps }
    },
    methods: {
        statusChange( e ) {
            let fileName = e.target.id
            let isStart = e.target.checked

            try {
                let data = fs.readFileSync(commands.autostartApps + '/' + fileName).toString()
                let check = data.match(/\X-GNOME-Autostart-enabled=.*/g)

                if ( check ){
                    data = data.replace(/\X-GNOME-Autostart-enabled=.*/gi, 'X-GNOME-Autostart-enabled='+ isStart)
                }
                else {
                    data += ('X-GNOME-Autostart-enabled=' + isStart + '\n')
                }

                fs.writeFileSync(commands.autostartApps + '/' + fileName, data)

                showMessage('Operation successfully.', 'success')
            }
            catch( err ) {
                console.log(err)
                showMessage('Operation failed.', 'error')
            }
        }
    }
})
