import si from 'systeminformation'
import { SemiCircle, Line } from 'progressbar.js'
import { prop } from '../config'
import helpers  from '../helpers'
//Components
import SystemInfo from './SystemInfo'
import UpBar from './UpBar'
import DownBar from './DownBar'
import CpuBar from './CpuBar'
import MemoryBar from './MemoryBar'
import DiskBar from './DiskBar'

				// <disk-bar/>
export default {
	template: `<div>
					<down-bar/>
					<system-info/>
					<up-bar/>

					<cpu-bar/>
					<memory-bar/>
					
					<!--Update Check-->
					<div class="fl w100 update-check" v-show="update_check">
						<span>There are updates currently available.</span>
						<button @click="download_update">
							Download Update
						</button>
					</div>
				</div>`,
	data() {
		return({
        	update_check: false
		})
	},
	components: {
		'system-info': SystemInfo,
		'up-bar': UpBar,
		'down-bar': DownBar,
		'cpu-bar': CpuBar,
		'memory-bar': MemoryBar,
		'disk-bar': DiskBar
	},
	created() {

	},
	methods: {
		// open the link on browser
        download_update() {
            shell.openExternal('https://github.com/oguzhaninan/Stacer/releases/latest')
        }
    }
}