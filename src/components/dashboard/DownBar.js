import {
	Line
} from 'progressbar.js'
import si from 'systeminformation'

export default {
	template: `<div class="line-cont">
					<h3>DOWNLOAD</h3>
					<div id="down-bar"></div>
					<span>{{ this.downSpeed + this.unit }}</span>
				</div>`,
	data() {
		return ({
			downSpeed: 0,
			unit:'KiB/s'
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
		// Get network nameMath.abs(data.rx_sec / 1024).toFixed(2) || 0.00
		si.networkInterfaceDefault(defaultNetwork => {
			setInterval(() => {
				// get down speed
				si.networkStats(defaultNetwork, data => {
					speedInKB = Math.abs(data.rx_sec / 1024).toFixed(2) || 0.00
					if(speedInKB>1024){
						speedInKB = speedInKB/1024
						this.unit = 'MiB/s'	
					}
					this.downSpeed = speedInKB
					// down bar update
					max = max < this.downSpeed ? this.downSpeed : max
					downBar.animate(this.downSpeed / max)
				})
			}, 1000)
		})
	}
}
