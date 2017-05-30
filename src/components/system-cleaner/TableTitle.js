export default {
	template: `<h3 v-show="length">
					<label>
						<slot></slot>
						<i></i>
						<span>{{ lang('selectAll') }} ({{text}})</span>
					</label>
				</h3>`,
	props: ['length', 'text']
}