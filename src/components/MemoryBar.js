import si from 'systeminformation'
import { SemiCircle } from 'progressbar.js'
import { prop } from '../config'
import helpers  from '../helpers'

export default {
	template: `<div class="cont fl" id="mem-cont">MEMORY</div>`,
	data() { return({ memInfo: 0 }) },
	mounted() {
		let memBar = new SemiCircle('#mem-cont' , {
			strokeWidth: 12,
			color: prop.memBarColor,
			trailColor: prop.trailColor,
			easing: 'easeInOut',
			duration: prop.memDuration,
			svgStyle: {
				marginTop: '10px'
			},
			text: {
				style: {
					color: '#fff',
					paddingBottom: '20px',
					position: 'absolute',
					left: '50%'
				}
			},
			step: ( state, bar ) => {
				bar.setText(this.memInfo)
			}
		})

		setInterval( () => {
			si.mem( ram => {
				let usedMem  = ram.total - ram.available
				let totalMem = ram.total
				this.memInfo = helpers.prettyMemSize(usedMem) + ' / ' + helpers.prettyMemSize(totalMem) + 'GB'
				memBar.animate(usedMem / totalMem)
			})
		}, prop.memDuration)
	}
}