import helpers from '../../utils/helpers'
import si from 'systeminformation'
import Chartkick from 'chartkick'
const seconds_max = 30;
export default {
	template: `<div>
					<h4>{{ lang('networkHistory') }} <slot></slot></h4>
			   		<div id="network-chart" :style="'height:' + fheight"></div>
				</div>`,
	props: ['fheight'],
	data() {
		return ({
			networkValues: [],
			networkData: [],
			seconds: Array.from(new Array(seconds_max),(val,index)=>index+1)
		})
	},
	mounted() {
		si.networkInterfaceDefault(defaultNetwork => {

			for (var i = 0; i < 2; i++)
				this.networkValues.push((new Array(seconds_max)).fill(0))

			let networkChart = new Chartkick.LineChart('network-chart', this.networkData, {
				colors: ['#2ecc71', '#e74c3c', '#3498db', '#f1c40f', '#9b59b6', '#34495e', '#1abc9c', '#e67e22'],
				legend: true,
				min: 0,
				points: false
			})

			setInterval(() => {
				si.networkStats(defaultNetwork, data => {
					let downSpeed = Math.abs(data.rx_sec / 1024).toFixed(2) || 0.00
					let upSpeed = Math.abs(data.tx_sec / 1024).toFixed(2) || 0.00

					this.networkValues.forEach((n, i) => this.networkValues[i].splice(0, 1))

					this.networkValues[0].push(downSpeed > 0 ? downSpeed : 0)
					this.networkValues[1].push(upSpeed > 0 ? upSpeed : 0)

					this.networkData = []

					this.networkData.push({
						name: lang('download'),
						data: this.networkValues[0].map((d, i) => [this.seconds[i], d])
					})

					this.networkData.push({
						name: lang('upload'),
						data: this.networkValues[1].map((d, i) => [this.seconds[i], d])
					})

					networkChart.updateData(this.networkData)
				})
			}, 1000)
		})
	}
}
