import si from 'systeminformation'
import Chartkick from 'chartkick'
const seconds_max = 30;
export default {
	template: `<div>
					<h4>{{ lang('cpuHistory') }} <slot></slot></h4>
			   		<div id="cpus-chart" :style="'height:' + fheight"></div>
				</div>`,
	props: ['fheight'],
	data() {
		return ({
			cpuValues: [],
			cpuData: [],
			seconds: Array.from(new Array(seconds_max),(val,index)=>index+1)
		})
	},
	mounted() {
		si.currentLoad(val => {
			let cpuCount = val.cpus.length

			for (var i = 0; i < cpuCount; i++)
				this.cpuValues.push((new Array(seconds_max)).fill(0))

			let cpuChart = new Chartkick.LineChart('cpus-chart', this.cpuData, {
				colors: ['#2ecc71', '#e74c3c', '#3498db', '#f1c40f', '#9b59b6', '#34495e',
					 '#1abc9c', '#e67e22', '#46f0f0', '#f032e6', '#fabebe', '#008080',
					 '#e6beff', '#aa6e28', '#d2f53c', '#800000', '#aaffc3', '#808000',
					 '#000080', '#808080'],
				legend: true,
				min: 0,
				max: 100,
				points: false
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
