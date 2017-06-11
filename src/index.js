import App from './components/App'
import Language from './languages/Language'
import config from './utils/config'
import winston from 'winston'

const userData = require('electron').remote.app.getPath('userData')

window.vm = new Vue({
    el: 'app',
    components: {
        'app': App
    },
    beforeCreate() {
        // Set configs
        config()
        // Check Language
        let appLanguage = localStorage.appLanguage || 'en'
        let language = new Language(appLanguage)
        window.lang = (key) => language.get(key)
        // Check Theme
        let appTheme = localStorage.appTheme || 'dark'
        document.body.classList = [appTheme]

        // Logger settings
        window.logger = new(winston.Logger)({
            transports: [
                new(winston.transports.File)({
                    filename: userData + '/stacer.log'
                })
            ]
        })
    },
    mounted() {
        setTimeout(() => document.getElementById('loading').remove(), 2500)
    }
})