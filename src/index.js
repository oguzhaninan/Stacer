import App from './components/App'
import Language from './languages/Language'

window.vm = new Vue({
    el: 'app',
    components: {
        'app': App
    },
    beforeCreate() {
        // Check Language
        let appLanguage = localStorage.getItem('appLanguage') || 'en'
        let language = new Language(appLanguage)
        window.lang = (key) => language.get(key)
        // Check Theme
        let appTheme = localStorage.getItem('appTheme') || 'dark'
        document.body.classList = [appTheme]
    },
    mounted() {
        setTimeout(() => document.getElementById('loading').remove(), 20)
    }
})