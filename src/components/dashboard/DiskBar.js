import {
	Path
} from 'progressbar.js'
import si from 'systeminformation'
import helpers from '../../utils/helpers'
import CircleBar from './CircleBar'

export default {
	template: `<circle-bar title="DISK SPACE" idd="disk-cont" stroke="#e74c3c" :text="this.diskUsed + ' / ' + this.diskTotal + 'GB'" img="disk.png" />`,
	components: {
		'circle-bar': CircleBar
	},
	data() {
		return ({
			diskUsed: 0,
			diskTotal: 0,
			diskBar: null
		})
	},
	mounted() {
		this.diskBar = new Path('#disk-cont', {
			duration: 5000
		})

		this.diskInfo()
		setInterval(this.diskInfo, 5000)
	},
	methods: {
		diskInfo() {
			si.fsSize(disk => {
				this.diskUsed = helpers.prettyDiskSize(disk[0].used)
				this.diskTotal = helpers.prettyDiskSize(disk[0].size)
				this.diskBar.animate(disk[0].use / 100)
			})
		}
	}
}