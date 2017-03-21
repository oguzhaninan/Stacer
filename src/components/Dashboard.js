import si from 'systeminformation'
import { SemiCircle, Line } from 'progressbar.js'
import { prop } from '../config'
import helpers  from '../helpers'

import SystemInfo from './SystemInfo'

export default {
	template: `<div>
				<circle-bar text="CPU" id="cpu-cont"></circle-bar>
				<circle-bar text="MEMORY" id="mem-cont"></circle-bar>
				<circle-bar text="DISK" id="disk-cont"></circle-bar>
				<line-bar name="down-bar" text="DOWNLOAD"></line-bar>
				<system-info/>
				<line-bar name="up-bar" text="UPLOAD"></line-bar>
				
				<!--Update Check-->
				<div class="fl w100 update-check" v-show="update_check">
					<span>There are updates currently available.</span>
					<button @click="download_update">
						Download Update
					</button>
				</div>
			</div>`,
	components: {
		'system-info': SystemInfo
	}

}