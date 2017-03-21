import si from 'systeminformation'
import { SemiCircle, Line } from 'progressbar.js'
import { prop } from '../config'

export default {
	template: `<div class="line-cont fr">
					<span>DOWNLOAD</span>
					<div id="down-bar"></div>
				</div>`,
	data(){
		return({ downSpeed: 0 })
	},
	mounted() {
		let downBar = new Line('#down-bar', {
			strokeWidth: 5,
			easing: 'easeInOut',
			duration: prop.networkBarsDuration,
			color: prop.netwrokBarColor,
			trailColor: prop.trailColor,
			text: {
				style: {
					color: '#999',
					position: 'absolute',
					right: '0',
					marginTop: '5px'
				}
			},
			step: (state, bar) => {
				bar.setText(Math.abs(this.downSpeed).toString() + ' kB/s')
			}
		})

		// Get network name
		si.networkInterfaceDefault( defaultNetwork => {
			setInterval( () => {				
				si.networkStats(defaultNetwork, data => {
					this.downSpeed = (data.rx_sec / 1024).toFixed(2)
					// down bar update
					downBar.animate(this.downSpeed / 2000)
				}) 
			}, prop.networkBarsDuration)
		})
	}
}