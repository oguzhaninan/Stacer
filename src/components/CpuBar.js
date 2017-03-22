import si from 'systeminformation'
import {
	SemiCircle
} from 'progressbar.js'
import {
	prop
} from '../utils/config'

export default {
	template: `<div class="cont fl" id="cpu-cont">CPU</div>`,
	mounted() {
		let cpuBar = new SemiCircle('#cpu-cont', {
			strokeWidth: 12,
			color: prop.cpuBarColor,
			trailColor: prop.trailColor,
			easing: 'easeInOut',
			duration: prop.cpuDuration,
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
			step: (state, bar) => {
				bar.setText(Math.round(bar.value() * 100) + "%")
			}
		})

		setInterval(() => {
			si.currentLoad(val => {
				cpuBar.animate(val.currentload / 100)
			})
		}, prop.cpuDuration)
	}
}