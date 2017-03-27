import 'chart.js'
import Chartkick from 'chartkick'
import si from 'systeminformation'

export default {
	template: `<transition name="slide-fade">
				<div class="content">
					<div class="statistics">
						<div id="cpus-chart"></div>
					</div>
				</div>
			</transition>`,
	data() {
		return ({
			cpu1Values: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					 	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
			cpu2Values: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
						 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],

			labels: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
				   16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30],
			data: [],
		})
	},
	mounted() {
		let chart = new Chartkick.LineChart('cpus-chart', this.data, {
			legend: true,
			min: 0,
			max: 100,
			colors: ['#2ecc71', '#e74c3c']
		})

		setInterval(()=> {
			si.currentLoad(val => {
				let cpuCount = val.cpus.length
				this.cpu1Values.splice(0, 1)
				this.cpu2Values.splice(0, 1)
				this.cpu1Values.push(val.cpus[0].load)
				this.cpu2Values.push(val.cpus[1].load)
				this.data = []
				this.data.push({name: 'Cpu1', data: this.cpu1Values.map((d, i) => [this.labels[i], d]) })
				this.data.push({name: 'Cpu2', data: this.cpu2Values.map((d, i) => [this.labels[i], d]) })
			})

			chart.updateData(this.data)
		}, 1000)
	}
}