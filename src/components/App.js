import Dashboard from './Dashboard'
import StartupApps from './StartupApps'
import Services from './Services'
import Uninstaller from './Uninstaller'
import SystemCleaner from './SystemCleaner'

export default {
	template: `<div id="main" class="fl w100">

					<ul id="nav">
						<li><label :class="{'nav-active': activeNav == 1}" @click="activeNav = 1">Dashboard</label></li>
						<li><label :class="{'nav-active': activeNav == 2}" @click="activeNav = 2">System Cleaner</label></li>
						<li><label :class="{'nav-active': activeNav == 3}" @click="activeNav = 3">Startup Apps</label></li>
						<li><label :class="{'nav-active': activeNav == 4}" @click="activeNav = 4">Services</label></li>
						<li><label :class="{'nav-active': activeNav == 5}" @click="activeNav = 5">Uninstaller</label></li>
					</ul>

					<!--Dashboard Page-->
					<transition name="slide-fade">
						<div v-show="activeNav == 1" class="content">
							<dashboard/>
						</div>
					</transition>

					<!--System Cleaner Page-->
					<transition name="slide-fade">
						<div v-show="activeNav == 2" class="content">
							<system-cleaner/>
						</div>
					</transition>

					<!--System Startup Apps Page-->
					<transition name="slide-fade">
						<div v-show="activeNav == 3" class="content">				
							<startup-apps/>
						</div>
					</transition>

					<!--Services Page-->
					<transition name="slide-fade">
						<div v-show="activeNav == 4" class="content">
							<services/>
						</div>
					</transition>

					<!--Packages Page-->
					<transition name="slide-fade">
						<div v-show="activeNav == 5" class="content">
							<uninstaller/>
						</div>
					</transition>

				</div>`,
	data() {
		return({ activeNav: 1 })
	},
	components: {
        'dashboard': Dashboard,
        'system-cleaner': SystemCleaner,
        'startup-apps': StartupApps,
        'services': Services,
        'uninstaller': Uninstaller,
    }
}