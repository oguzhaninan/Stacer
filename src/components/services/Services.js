import {
	spawnSync
} from 'child_process'
import {
	command,
	showMessage
} from '../../utils/helpers'
import si from 'systeminformation'
// components
import Service from './Service'

export default {
	template: `<transition name="slide-fade">
				<div class="content">
					<slot></slot>
					<div class="item-list services-list">
						<div class="fl w100">
							<h3 class="fl">{{ lang('serviceTitle') }} ({{ filteredServices.length }})</h3>
							<input type="text" v-model="searchString" :placeholder="lang('search') + '...'" />
						</div>
						<ul v-show="filteredServices.length" class="scroll">
							<service v-for="s in filteredServices" :name="s.name" :is-run="s.isRun" />
						</ul>
						<span class="empty-list" v-show="! filteredServices.length">
							{{ lang('noService') }}
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
		const services = spawnSync('bash', ['-c', localStorage.getAllService])

		let serviceNames = services.stdout.toString().trim().split('\n')
		
		serviceNames.forEach(serviceName => {
			const isActive = spawnSync('systemctl', ['is-active', serviceName]).stdout.toString().trim()
			
			let checked = isActive === 'active' ? 'checked' : ''
			
			this.servicesList.push({
				name: serviceName,
				isRun: checked
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