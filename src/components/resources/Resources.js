import 'chart.js'
import Chartkick from 'chartkick'
import si from 'systeminformation'

export default {
	template: `<transition name="slide-fade">
				<div class="content">
					<div class="resources">
						<h4>Cpu History</h4>
						<div id="cpus-chart"></div>
					</div>
				</div>
			</transition>`,
	data() {
		return ({
			cpuValues: [],

			labels: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
				   16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30],
			data: [],
		})
	},
	mounted() {
		si.currentLoad(val => {
			let cpuCount = val.cpus.length

			for (var i = 0; i < cpuCount; i++)
				this.cpuValues.push([0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
									 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
			
			let chart = new Chartkick.LineChart('cpus-chart', this.data, {
				legend: true,
				min: 0,
				max: 100,
				colors: ['#2ecc71', '#e74c3c', '#3498db', '#f1c40f']
			})

			setInterval(()=> {
				si.currentLoad(val => {
					this.cpuValues.forEach((cpu, i) => this.cpuValues[i].splice(0, 1))
					this.cpuValues.forEach((cpu, i) => this.cpuValues[i].push(val.cpus[i].load.toFixed(1)))
					this.data = []
					this.cpuValues.forEach((cpu, i) => {
						let name = 'Cpu' + (i+1) + ' ' + val.cpus[i].load.toFixed(1) + '%'
						this.data.push({name: name, data: this.cpuValues[i].map((d, i) => [this.labels[i], d]) })
					})
					chart.updateData(this.data)
				})
			}, 1000)
		})
	}
}