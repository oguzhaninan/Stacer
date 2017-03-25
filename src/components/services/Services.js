import sudo from 'sudo-prompt'
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

import Service from './Service'

export default {
	template: `<transition name="slide-fade">
				<div class="content">
					<div class="item-list services-list">
						<div class="fl w100">
							<h3 class="fl">System Services ({{ filteredServices.length }})</h3>
							<input type="text" v-model="searchString" placeholder="Search..." />
						</div>
						<ul v-show="filteredServices.length" class="scroll">
							<service v-for="s in filteredServices" :name="s.name" :is-run="s.isRun" />
						</ul>
						<span class="empty-list" v-show="! filteredServices.length">
							No service found.
						</span>
					</div>
				</div>
			</transition>`,
	components: {
		'service': Service
	},
	data() {
		return ({
			servicesList: [],
			searchString: '',
			isBusy: false
		})
	},
	created() {
		const services = spawn('bash', ['-c', commands.getAllService])

		services.stdout.on('data', data => {
			data = data.toString().split('\n').filter(s => s != '')

			data.forEach(service => {
				var serviceName = service.substring(1)
				var isRun = service.substring(0, 1) == '+' ? 'checked' : ''
				this.servicesList.push({
					name: serviceName,
					isRun: isRun
				})
			})
		})
	},
	methods: {
		// Service status change switch button function
		statusChange(e) {
			let serviceName = e.target.id
			let status = e.target.checked ? 'start' : 'stop'

			if (!this.isBusy) {
				this.isBusy = true
				sudo.exec(command(`service ${serviceName} ${status}`), {
						name: 'Stacer'
					},
					(error, stdout, stderr) => {
						if (stderr) {
							e.target.checked = !status
							showMessage('Operation not successful.', 'error')
						} else {
							showMessage(serviceName + ' service ' + status + (e.target.checked ? 'ed' : 'ped'), 'success')
						}

						this.isBusy = false
					})
			} else {
				showMessage('Another process continues.', 'error')
			}
		}
	},
	computed: {
		// Search services
		filteredServices() {
			let tempServicesList = this.servicesList,
				searchString = this.searchString

			if (!searchString) return tempServicesList

			searchString = searchString.toString().trim().toLowerCase()

			return tempServicesList.filter(item =>
				item.name.toString().trim().toLowerCase().indexOf(searchString) !== -1
			)
		}
	}
}