import {
	Path
} from 'progressbar.js'
import si from 'systeminformation'
import CircleBar from './CircleBar'

export default {
	template: `<circle-bar title="CPU" idd="cpu-cont" stroke="#2ecc71" :text="cpuValue + '%'" img="cpu.png" />`,
	components: {
		'circle-bar': CircleBar
	},
	data() {
		return ({
			cpuValue: 0
		})
	},
	mounted() {
		let cpuBar = new Path('#cpu-cont', {
			duration: 1000
		})

		setInterval(() => {
			si.currentLoad(val => {
				this.cpuValue = val.currentload.toFixed(0)
				cpuBar.animate(this.cpuValue / 100)
			})
		}, 1000)
	}
}