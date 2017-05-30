import LanguageList from '../../languages/lang-list'
import ThemeList from './theme-list'
import { showMessage } from '../../utils/helpers'

export default {
	template: `<transition name="slide-fade">
							<div class="content settings">
								<h3>{{ lang('appSettings') }}</h3>
								<div>
									<label>{{ lang('appLanguage') }}</label>
									<select v-model="selectedLang" @change="langChange">
										<option :value="k" v-for="(v, k) in langList">{{v}}</option>
									</select>
								</div>
								<div>
									<label>{{ lang('appTheme') }}</label>
									<select v-model="selectedTheme" @change="themeChange">
										<option :value="k" v-for="(v, k) in themeList">{{v}}</option>
									</select>
								</div>
							</div>
						</transition>`,
	data() {
		return({
			langList: LanguageList,
			selectedLang: '',

			themeList: ThemeList,
			selectedTheme: ''
		})
	},
	created() {
		this.selectedLang = localStorage.getItem('appLanguage') || 'en'
		this.selectedTheme = localStorage.getItem('appTheme') || 'dark'
	},
	methods: {
		langChange() {
			localStorage.setItem('appLanguage', this.selectedLang)
			showMessage(lang('changedLang'), 'warning')
		},
		themeChange() {
			localStorage.setItem('appTheme', this.selectedTheme)
			document.body.classList = [this.selectedTheme]
		}
	}
}
