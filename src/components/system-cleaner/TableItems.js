export default {
	template: `<li>
					<label>
						<slot></slot>
						<i></i>
						<span> {{ text }} </span>
					</label>
				</li>`,
	props: ['text']
}