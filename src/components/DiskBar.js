import si from 'systeminformation'
import { SemiCircle } from 'progressbar.js'
import { prop } from '../config'
import helpers  from '../helpers'

export default {
	template: `<div class="cont fl" id="disk-cont">DISK</div>`,
	data() { 
		return({
			dUsed: 0,
			dTotal: 0,
			diskBar: null
		})
	},
	mounted() {
		this.diskBar = new SemiCircle('#disk-cont' , {
			strokeWidth: 12,
			color: prop.diskBarColor,
			trailColor: prop.trailColor,
			easing: 'easeInOut',
			duration: prop.diskDuration,
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
				bar.setText(this.dUsed + ' / ' + this.dTotal + 'GB')
			}
		})

		this.diskInfo()
  		setInterval(this.diskInfo, prop.diskDuration)
	},
	methods: {
		diskInfo () {
			si.fsSize( disk => {
				this.dUsed  = helpers.prettyDiskSize( disk[0].used )
				this.dTotal = helpers.prettyDiskSize( disk[0].size )
				this.diskBar.animate( disk[0].use / 100 )
			})
		}
	}
}