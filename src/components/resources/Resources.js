import 'chart.js'
Chart.defaults.global.defaultFontColor = '#aaa'
Chart.defaults.global.defaultFontFamily = 'Ubuntu'
// Components
import CpuHistory from './CpuHistory'
import MemoryHistory from './MemoryHistory'
import NetworkHistory from './NetworkHistory'

export default {
	template: `<transition name="slide-fade">
				<div class="content">
					<slot></slot>
					<div class="resources">
						<cpu-history v-show="fitChart == 0 || fitChart == 1" :fheight="fitHeight">
							<button :class="['fit-chart', { 'collapse' : isFit }]" @click="fit(1)" />
						</cpu-history>
						<memory-history v-show="fitChart == 0 || fitChart == 2" :fheight="fitHeight">
							<button :class="['fit-chart', { 'collapse' : isFit }]" @click="fit(2)" />
						</memory-history>
						<network-history v-show="fitChart == 0 || fitChart == 3" :fheight="fitHeight">
							<button :class="['fit-chart', { 'collapse' : isFit }]" @click="fit(3)" />
						</network-history>
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
			fitChart: 0,
			fitHeight: '125px',
			isFit: false
		})
	},
	methods: {
		fit(val) {
			if (this.fitChart == val) {
				this.fitChart = 0
				this.isFit = false
			} else {
				this.fitChart = val
				this.isFit = true
			}
			this.fitHeight = this.fitChart == 0 ? '125px' : '500px'
		}
	}
}