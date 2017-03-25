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
import si from 'systeminformation'

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
			searchString: ''
		})
	},
	created() {
		si.processes(pr => {
			console.log(pr)
		})

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