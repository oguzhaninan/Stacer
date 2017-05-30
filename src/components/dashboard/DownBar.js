import {
	Line
} from 'progressbar.js'
import si from 'systeminformation'

export default {
	template: `<div class="line-cont">
					<h3>{{ lang('download') }}</h3>
					<div id="down-bar"></div>
					<span>{{ downSpeed + ' kB/s'  }}</span>
				</div>`,
	data() {
		return ({
			downSpeed: 0
		})
	},
	mounted() {
		let downBar = new Line('#down-bar', {
			strokeWidth: 5,
			duration: 1000,
			color: '#3498db',
			trailColor: '#202b33'
		})

		let max = 1000
		// Get network name
		si.networkInterfaceDefault(defaultNetwork => {
			setInterval(() => {
				// get down speed
				si.networkStats(defaultNetwork, data => {
					let speed = Math.abs(data.rx_sec / 1024).toFixed(2)
					this.downSpeed = speed > 0 ? speed : 0
					// down bar update
					max = max < this.downSpeed ? this.downSpeed : max
					let percent = this.downSpeed / max < 1 ? this.downSpeed / max : 1
					downBar.animate(percent)
				})
			}, 1000)
		})
	}
}
