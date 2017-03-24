import {
	commands
} from '../utils/config'
import {
	showMessage
} from '../utils/helpers'
import chokidar from 'chokidar'
import fs from 'fs'
import properties from 'properties-reader'

export default {
	template: `<div id="startup-apps-table">
					<div id="startup-apps-title">
						<span style="padding:0;">System Startup Applications ({{ apps.length }})</span>
					</div>
					<div class="tdl-content scroll">
						<span class="fl w100 empty-list" v-show="! apps.length">
							No startup apps found.
						</span>
						<ul v-show="apps.length">
							<li v-for="app in apps">
								<span>{{ app.name }}</span>
								<input type="checkbox" class="switch" :id="app.file" :checked="app.isStart" @change="statusChange" />
								<label :for="app.file"></label>
								<button :name="app.file" @click="removeApp" class="remove-startup-app"></button>
							</li>
						</ul>
					</div>
					<button @click="showPrompt = true" class="add-startup-app">Add Startup App</button>
					
					<div class="promptDialog" v-show="showPrompt">
						<div class="dialog">
							<div>
								<span>Application</span>
								<input type="text" v-model="appName" placeholder="App Name" />
								<input type="text" v-model="appComment" placeholder="App Comment" />
								<input type="text" v-model="appExec" placeholder="Command" />
								<button @click="saveApp">Add</button>
								<button @click="cancelPrompt">Cancel</button>
							</div>
						</div>
					</div>
				</div>`,
	data() {
		return ({
			apps: [],
			showPrompt: false,
			appName: '',
			appComment: '',
			appExec: ''
		})
	},
	created() {
		this.getApps()

		chokidar.watch(commands.autostartApps, {
				persistent: true,
				ignoreInitial: true
			})
			.on('add', path => this.getApps())
			.on('unlink', path => this.getApps())
	},
	methods: {
		removeApp(e) {
			fs.unlinkSync(commands.autostartApps + e.target.name)
		},
		saveApp() {
			if (this.appName && this.appComment && this.appExec) {
				let desktopFile = `[Desktop Entry]
									\rName=${this.appName}
									\rExec=${this.appExec}
									\rComment=${this.appComment}
									\rType=ApplicationR
									\rTerminal=false
									\rX-GNOME-Autostart-enabled=true`

				try {
					fs.writeFileSync(commands.autostartApps + this.appName + '.desktop', desktopFile)
				} catch (err) {

				} finally {
					this.cancelPrompt()
				}
			} else {
				showMessage('Do not leave required fields blank.', 'error')
			}
		},
		cancelPrompt() {
			this.showPrompt = false
			this.appName = this.appComment = this.appExec = ''
		},
		getApps() {
			try {
				fs.readdir(commands.autostartApps, (err, files) => {
					if (!err) {
						this.apps.splice(0, this.apps.length) // array clear
						files.filter(file => file.endsWith('.desktop')).forEach(file => {
							try {
								var entry = properties(commands.autostartApps + '/' + file)

								if (entry.get('Desktop Entry.Name') != null) {
									let appName = entry.get('Desktop Entry.Name')
									let isStart = entry.get('Desktop Entry.X-GNOME-Autostart-enabled')

									if (appName != null) {
										this.apps.push({
											name: appName,
											file: file,
											isStart: (isStart != null ? isStart : true)
										})
									}
								}
							} catch (err) {}
						})
					}
				})
			} catch (error) {
				console.log(error)
			}
		},
		statusChange(e) {
			let fileName = e.target.id
			let isStart = e.target.checked

			try {
				let data = fs.readFileSync(commands.autostartApps + '/' + fileName).toString()
				let check = data.match(/\X-GNOME-Autostart-enabled=.*/g)

				if (check) {
					data = data.replace(/\X-GNOME-Autostart-enabled=.*/gi, 'X-GNOME-Autostart-enabled=' + isStart)
				} else {
					data += ('X-GNOME-Autostart-enabled=' + isStart + '\n')
				}

				fs.writeFileSync(commands.autostartApps + '/' + fileName, data)
			} catch (err) {
				console.log(err)
				showMessage('Operation failed.', 'error')
			}
		}
	}

}