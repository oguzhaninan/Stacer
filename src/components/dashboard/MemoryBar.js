import {
	Path
} from 'progressbar.js'
import si from 'systeminformation'
import helpers from '../../utils/helpers'
import CircleBar from './CircleBar'

export default {
	template: `<circle-bar :title="lang('memory')" idd="mem-cont" stroke="#f1c40f" :text="memoryValue" img="memory.png" />`,
	components: {
		'circle-bar': CircleBar
	},
	data() {
		return ({
			memoryValue: 0
		})
	},
	mounted() {
		let memoryBar = new Path('#mem-cont', {
			duration: 1500
		})

		setInterval(() => {
			si.mem(ram => {
				let usedMem = ram.total - ram.available
				let totalMem = ram.total
				this.memoryValue = helpers.prettyMemSize(usedMem) + ' / ' + helpers.prettyMemSize(totalMem) + 'GB'
				memoryBar.animate(usedMem / totalMem)
			})
		}, 1500)
	}
}