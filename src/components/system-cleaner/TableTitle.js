export default {
	template: `<h3 v-show="length">
					<label>
						<slot></slot>
						<i></i>
						<span>Select All ({{text}})</span>
					</label>
				</h3>`,
	props: ['length', 'text']
}