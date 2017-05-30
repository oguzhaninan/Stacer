import {
	Line
} from 'progressbar.js'
import si from 'systeminformation'

export default {
	template: `<div class="line-cont">
					<h3>{{ lang('upload') }}</h3>
					<div id="up-bar"></div>
					<span>{{ this.upSpeed + ' kB/s' }}</span>
				</div>`,
	data() {
		return ({
			upSpeed: 0
		})
	},
	mounted() {
		let upBar = new Line('#up-bar', {
			strokeWidth: 5,
			duration: 1000,
			color: '#3498db',
			trailColor: '#202b33'
		})

		let max = 1000
		// Get network name
		si.networkInterfaceDefault(defaultNetwork => {
			setInterval(() => {
				// get upload speed
				si.networkStats(defaultNetwork, data => {
					let speed = Math.abs(data.tx_sec / 1024).toFixed(2)
					this.upSpeed = speed > 0 ? speed : 0
					// up bar update
					max = max < this.upSpeed ? this.upSpeed : max
					let percent = this.upSpeed / max < 1 ? this.upSpeed / max : 1
					upBar.animate(percent)
				})
			}, 1000)
		})
	}
}