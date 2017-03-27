import si from 'systeminformation'
import Chartkick from 'chartkick'

export default {
	template: `<div>
					<h4>Cpu History</h4>
			   		<div id="cpus-chart"></div>
				</div>`,
	data() {
		return ({
			cpuValues: [],
			cpuData: [],

			seconds: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
				16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30
			],
		})
	},
	mounted() {
		si.currentLoad(val => {
			let cpuCount = val.cpus.length

			for (var i = 0; i < cpuCount; i++)
				this.cpuValues.push([0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
				])

			let cpuChart = new Chartkick.LineChart('cpus-chart', this.cpuData, {
				colors: ['#2ecc71', '#e74c3c', '#3498db', '#f1c40f', '#9b59b6', '#34495e', '#1abc9c', '#e67e22'],
				legend: true,
				min: 0,
				max: 100
			})

			setInterval(() => {
				si.currentLoad(val => {
					this.cpuValues.forEach((cpu, i) => this.cpuValues[i].splice(0, 1))
					this.cpuValues.forEach((cpu, i) => this.cpuValues[i].push(val.cpus[i].load.toFixed(1)))

					this.cpuData = []

					this.cpuValues.forEach((cpu, i) => {
						let name = 'CPU' + (i + 1) + ' ' + val.cpus[i].load.toFixed(1) + '%'
						this.cpuData.push({
							name: name,
							data: this.cpuValues[i].map((d, i) => [this.seconds[i], d])
						})
					})

					cpuChart.updateData(this.cpuData)
				})
			}, 1000)
		})
	}
}