import helpers from '../../utils/helpers'
import 'chart.js'
import Chartkick from 'chartkick'
import si from 'systeminformation'
// Components
import CpuHistory from './CpuHistory'
import MemoryHistory from './MemoryHistory'
import NetworkHistory from './NetworkHistory'

export default {
	template: `<transition name="slide-fade">
				<div class="content">
					<div class="resources">
						<cpu-history />
						<memory-history />
						<network-history />
					</div>
				</div>
			</transition>`,
	components: {
		'cpu-history': CpuHistory,
		'memory-history': MemoryHistory,
		'network-history': NetworkHistory
	},
	data() {
		return ({
			
		})
	},
	mounted() {
		

	}
}