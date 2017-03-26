export default {
	template: `<h3 v-show="length">
					<label>
						<slot></slot>
						<i></i>
						Select All ({{text}})
					</label>
				</h3>`,
	props: ['length', 'text']
}