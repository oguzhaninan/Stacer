
import Vuex from 'Vuex'

import app from './modules/app'
import dashboard from './modules/dashboard'
import services from './modules/services'
import startupApps from './modules/startup-apps'
import systemCleaner from './modules/system-cleaner'
import uninstaller from './modules/uninstaller'

export default new Vuex.store({
	modules: {
		app,
		dashboard,
		services,
		startupApps,
		systemCleaner,
		uninstaller
	}	
})
