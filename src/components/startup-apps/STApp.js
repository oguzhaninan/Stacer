import fs from 'fs'
import {
	showMessage
} from '../../utils/helpers'

export default {
	template: `<li>
					<span>{{ name }}</span>
					<input type="checkbox" class="switch" :id="file" :checked="isStart" @change="statusChange" />
					<label :for="file" class="fr"></label>
					<button :name="file" @click="removeApp" class="remove-startup-app"></button>
					<button @click="editApp" class="edit-startup-app"></button>
				</li>`,
	props: ['name', 'file', 'is-start'],
	methods: {
		removeApp(e) {
            fs.unlinkSync(localStorage.autostartApps + e.target.name)
		},
		editApp(){
			//Emit an event to startupApps
			this.$emit('edit',this.name)
		},
		statusChange(e) {
			let fileName = e.target.id
			let isStart = e.target.checked

			try {
				let data = fs.readFileSync(localStorage.autostartApps + '/' + fileName).toString()
				let check = data.match(/\X-GNOME-Autostart-enabled=.*/g)

				if (check)
					data = data.replace(/\X-GNOME-Autostart-enabled=.*/gi, 'X-GNOME-Autostart-enabled=' + isStart)
				else
					data += ('X-GNOME-Autostart-enabled=' + isStart + '\n')

				fs.writeFileSync(localStorage.autostartApps + '/' + fileName, data)
			} catch (err) {
				logger.error('StartupApps Status Change', stderr)
				showMessage(lang('opFail'), 'error')
			}
		}
	}
}