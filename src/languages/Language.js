import langList from './lang-list'
export default class Language {

	constructor(lang) {
		if (Object.keys(langList).indexOf(lang) !== -1) {
			this.lang = require(`./langs/lang.${lang}`)
		} else {
			this.lang = require('./langs/lang.en')
		}
	}

	get(key) {
		return this.lang.default[key]
	}
}