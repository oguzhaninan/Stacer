export default {
	template: `<div class="cont fl">
					<h3>{{ title }}</h3>
					<svg viewBox="0 0 100 73" class="circle-bar" :style="'background-image: url(../assets/img/' + img + ')'">
						<path d="M 55,71 m -45,0 a 45,45 0 1 1 80,0" stroke="#202b33" stroke-width="10" fill-opacity="0" />
						<path :id="idd" d="M 55,71 m -45,0 a 45,45 0 1 1 80,0" :stroke="stroke" stroke-width="10" fill-opacity="0" />
					</svg>
					<span>{{ text }}</span>
				</div>`,
	props: ['stroke', 'text', 'idd', 'img', 'title']
}