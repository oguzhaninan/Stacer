import Dashboard from './dashboard/Dashboard'
import SystemCleaner from './system-cleaner/SystemCleaner'
import StartupApps from './startup-apps/StartupApps'
import Services from './services/Services'
import Uninstaller from './uninstaller/Uninstaller'
import Resources from './resources/Resources'

export default {
	template: `<div id="main" class="fl w100">

					<ul id="nav">
						<li><label :class="{'nav-active': activeNav == 1}" @click="activeNav = 1">Dashboard</label></li>
						<li><label :class="{'nav-active': activeNav == 2}" @click="activeNav = 2">System Cleaner</label></li>
						<li><label :class="{'nav-active': activeNav == 3}" @click="activeNav = 3">Startup Apps</label></li>
						<li><label :class="{'nav-active': activeNav == 4}" @click="activeNav = 4">Services</label></li>
						<li><label :class="{'nav-active': activeNav == 5}" @click="activeNav = 5">Uninstaller</label></li>
						<li><label :class="{'nav-active': activeNav == 6}" @click="activeNav = 6">Resources</label></li>
					</ul>
					
					<dashboard v-show="activeNav == 1" />

					<!--System Cleaner Page-->
					<transition name="slide-fade">
						<div v-show="activeNav == 2" class="content">
							<system-cleaner/>
						</div>
					</transition>

					<startup-apps v-show="activeNav == 3" />

					<services v-show="activeNav == 4" />
		
					<uninstaller v-show="activeNav == 5" />

					<resources v-show="activeNav == 6" />

					<!--Startup Loading-->
					<div id="loading">
						<span class="loader_text"> Loading... </span>
						<div id="loader">
							<span></span><span></span><span></span>
							<span></span><span></span><span></span>
							<span></span><span></span><span></span>
							<span></span><span></span><span></span>
							<span></span><span></span><span></span>
							<span></span><span></span><span></span>
							<span></span><span></span>
						</div>
					</div>
				</div>`,
	data() {
		return ({
			activeNav: 6
		})
	},
	components: {
		'dashboard': Dashboard,
		'system-cleaner': SystemCleaner,
		'startup-apps': StartupApps,
		'services': Services,
		'uninstaller': Uninstaller,
		'resources': Resources
	}
}