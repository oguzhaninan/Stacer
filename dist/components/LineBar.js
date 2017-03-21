'use strict';

Object.defineProperty(exports, "__esModule", {
	value: true
});
exports.default = {
	template: '<div class="line-cont fr">\n\t\t\t\t<span>{{ text }}</span>\n\t\t\t\t<div v-bind:id="name"></div>\n\t\t\t</div>',
	props: ['text', 'name']
};