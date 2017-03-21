import si from 'systeminformation'
import { SemiCircle, Line } from 'progressbar.js'
import { prop } from '../config'

export default {
	template: `<div class="line-cont fr">
					<span>UPLOAD</span>
					<div id="up-bar"></div>
				</div>`,
	data(){
		return({ upSpeed: 0 })
	},
	mounted() {
		let upBar = new Line('#up-bar', {
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
			step: ( state, bar ) => {
				bar.setText(Math.abs(this.upSpeed).toString() + ' kB/s')
			}
		})

		// Get network name
		si.networkInterfaceDefault( defaultNetwork => {
			setInterval( () => {				
				si.networkStats(defaultNetwork, data => {
					this.upSpeed = (data.tx_sec / 1024).toFixed(2)
					// up bar update
					upBar.animate(this.upSpeed / 2000)
				}) 
			}, prop.networkBarsDuration)
		})
	}
}