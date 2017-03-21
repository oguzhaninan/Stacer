import dashboard from './dashboard'
import startupApps from './startup-apps'
import Services from './components/Services'
import Uninstaller from './components/Uninstaller'
import systemCleaner from './system-cleaner'

import { shell } from 'child_process'

new Vue({
    el: '#main',
    data: {
        activeNav: 1,
        update_check: false
    },
    components: {
        'uninstaller': Uninstaller,
        'services': Services
    },
    methods: {
        download_update() {
            // open the link on browser
            shell.openExternal('https://github.com/oguzhaninan/Stacer/releases/latest')
        }
    },
    mounted() {
        /**
         * Update check
         */
        try {
            $.getJSON('https://api.github.com/repos/oguzhaninan/Stacer/releases/latest', ( data ) => {
                let currentVersion = require('../package.json').version.toString()
                let releaseVersion = data.tag_name.substr(1).toString()

                this.update_check = (currentVersion != releaseVersion)
            })
        }
        catch(error) { console.log(error) }

        /**
         * Dashboard Page
         */
        dashboard.systemBars()
        dashboard.networkBars()
    }
})

setTimeout(() => {
    $('#loading').remove()
}, 3000);
