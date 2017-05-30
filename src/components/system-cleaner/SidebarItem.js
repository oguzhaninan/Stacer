export default {
	template: `<li>
					<label>
						<slot></slot>
						<i></i>
						<span :title="text + '(' + length + ')'"> {{text}} ({{length}}) </span>
					</label>
				</li>`,
	props: ['length', 'text']
}