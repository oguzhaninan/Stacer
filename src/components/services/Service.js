import {
	command,
	showMessage
} from '../../utils/helpers'
import sudo from 'sudo-prompt'

export default {
	template: `<li>
					<span>{{ name }}</span>
					<input type="checkbox" class="switch" :id="name" :checked="isRun" @change="statusChange" />
					<label :for="name" class="fr"></label>
				</li>`,
	props: ['name', 'is-run'],
	data() {
		return ({
			isBusy: false
		})
	},
	methods: {
		statusChange(e) {
			let serviceName = e.target.id
			let isChecked = e.target.checked
			let status = isChecked ? 'start' : 'stop'

			if (!this.isBusy) {
				this.isBusy = true
				sudo.exec(command(`systemctl ${status} ${serviceName}`), {
						name: 'Stacer'
					},
					(error, stdout, stderr) => {
						if (stderr) {
							e.target.checked = ! isChecked
							showMessage(lang('opFail'), 'error')
						} else {
							showMessage(serviceName + lang('service').toLowerCase() + status + (e.target.checked ? 'ed' : 'ped'), 'success')
						}

						this.isBusy = false
					})
			} else {
				showMessage(lang('anotherProc'), 'error')
			}
		}
	}
}