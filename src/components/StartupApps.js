import { commands } from '../config'
import { showMessage } from '../helpers'
import chokidar from 'chokidar'
import fs from 'fs'
import properties from 'properties-reader'

export default {
	template: `<div id="startup-apps-table">
					<div id="startup-apps-title">
						<span style="padding:0;">System Startup Applications</span>
					</div>
					<div class="tdl-content scroll">
						<span class="fl w100 empty-list" v-show="! apps.length" >
							No startup apps found.
						</span>
						<ul v-show="apps.length">
							<li v-for="app in apps">
								{{ app.name }}
								<input type="checkbox" class="switch" :id="app.file" :checked="app.isStart" @change="statusChange" />
								<label :for="app.file"></label>
							</li>
						</ul>
					</div>
				</div>`,
	data() {
		return ({
			apps: []
		})
	},
	created() {
		this.getApps()
		chokidar.watch(commands.autostartApps, { persistent: true, ignoreInitial: true })
			.on('add', path => { this.getApps() })
			.on('unlink', path => { this.getApps() })
	},
	methods: {
		getApps() {
			try {
				fs.readdir( commands.autostartApps, ( err, files ) => {
					if( ! err ) {
						this.apps.splice(0, this.apps.length) // array clear
						files.filter( file => file.endsWith('.desktop') ).forEach( file => {
							try {
								var entry = properties(commands.autostartApps+ '/' + file)

								if ( entry.get('Desktop Entry.Name') != null ) {
									let appName = entry.get('Desktop Entry.Name')
									let isStart = entry.get('Desktop Entry.X-GNOME-Autostart-enabled')

									if ( appName != null ) {
										this.apps.push({
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
		},
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

}