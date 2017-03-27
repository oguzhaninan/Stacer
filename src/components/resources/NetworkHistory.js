import helpers from '../../utils/helpers'
import si from 'systeminformation'
import Chartkick from 'chartkick'

export default {
	template: `<div>
					<h4>Network History</h4>
			   		<div id="network-chart"></div>
				</div>`,
	data() {
		return ({
			networkValues: [],
			networkData: [],

			seconds: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
				16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30
			],
		})
	},
	mounted() {
		si.networkInterfaceDefault(defaultNetwork => {

			for (var i = 0; i < 2; i++)
				this.networkValues.push([0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
				])

			let networkChart = new Chartkick.LineChart('network-chart', this.networkData, {
				colors: ['#2ecc71', '#e74c3c', '#3498db', '#f1c40f', '#9b59b6', '#34495e', '#1abc9c', '#e67e22'],
				legend: true,
				min: 0
			})

			setInterval(() => {
				si.networkStats(defaultNetwork, data => {
					let downSpeed = Math.abs(data.rx_sec / 1024).toFixed(2) || 0.00
					let upSpeed = Math.abs(data.tx_sec / 1024).toFixed(2) || 0.00

					this.networkValues.forEach((n, i) => this.networkValues[i].splice(0, 1))

					this.networkValues[0].push(downSpeed)
					this.networkValues[1].push(upSpeed)

					this.networkData = []

					this.networkData.push({
						name: "Download",
						data: this.networkValues[0].map((d, i) => [this.seconds[i], d])
					})
					this.networkData.push({
						name: "Upload",
						data: this.networkValues[1].map((d, i) => [this.seconds[i], d])
					})

					networkChart.updateData(this.networkData)
				})
			}, 1000)
		})
	}
}