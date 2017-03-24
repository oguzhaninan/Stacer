import {
	commands
} from '../../utils/config'
import {
	showMessage
} from '../../utils/helpers'
import properties from 'properties-reader'
import chokidar from 'chokidar'
import fs from 'fs'
import STApp from './STApp'

export default {
	template: `<div id="startup-apps-tabl">
					<div id="startup-apps-title">
						<span style="padding:0;">System Startup Applications ({{ apps.length }})</span>
					</div>
					<div class="item-list scroll">
						<span class="empty-list" v-show="! apps.length">
							No startup apps found.
						</span>
						<ul v-show="apps.length">
							<app v-for="app in apps" :name="app.name" :file="app.file" :is-start="app.isStart"></app>
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

	components: {
		'app': STApp
	},
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
		}
	}

}