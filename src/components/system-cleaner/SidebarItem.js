export default {
	template: `<li>
					<label>
						<slot></slot>
						<i></i>
						<span> {{text}} ({{length}}) </span>
					</label>
				</li>`,
	props: ['length', 'text']
}