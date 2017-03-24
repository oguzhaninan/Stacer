export default {
	template: `<li>
					<span>{{ name }}</span>
					<input type="checkbox" class="switch" :id="file" :checked="isStart" @change="statusChange" />
					<label :for="file" class="fr"></label>
					<button :name="file" @click="removeApp" class="remove-startup-app"></button>
				</li>`,
	props: ['name', 'file', 'is-start'],
	methods: {
		removeApp(e) {
			fs.unlinkSync(commands.autostartApps + e.target.name)
		},
		statusChange(e) {
			let fileName = e.target.id
			let isStart = e.target.checked

			try {
				let data = fs.readFileSync(commands.autostartApps + '/' + fileName).toString()
				let check = data.match(/\X-GNOME-Autostart-enabled=.*/g)

				if (check)
					data = data.replace(/\X-GNOME-Autostart-enabled=.*/gi, 'X-GNOME-Autostart-enabled=' + isStart)
				else
					data += ('X-GNOME-Autostart-enabled=' + isStart + '\n')

				fs.writeFileSync(commands.autostartApps + '/' + fileName, data)
			} catch (err) {
				console.log(err)
				showMessage('Operation failed.', 'error')
			}
		}
	}
}