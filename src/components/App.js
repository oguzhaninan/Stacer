import Dashboard from './dashboard/Dashboard'
import SystemCleaner from './system-cleaner/SystemCleaner'
import StartupApps from './startup-apps/StartupApps'
import Services from './services/Services'
import Process from './process/Process'
import Uninstaller from './uninstaller/Uninstaller'
import Resources from './resources/Resources'
import Settings from './settings/Settings'

export default {
	template: `<div id="main" class="fl w100">
	
					<ul id="nav">
						<li><label :class="{'nav-active': activeNav == 1}" @click="activeNav = 1">{{ lang('dashboard') }}</label></li>
						<li><label :class="{'nav-active': activeNav == 2}" @click="activeNav = 2">{{ lang('sysCleaner') }}</label></li>
						<li><label :class="{'nav-active': activeNav == 3}" @click="activeNav = 3">{{ lang('startupApps') }}</label></li>
						<li><label :class="{'nav-active': activeNav == 4}" @click="activeNav = 4">{{ lang('services') }}</label></li>
						<li><label :class="{'nav-active': activeNav == 5}" @click="activeNav = 5">{{ lang('process') }}</label></li>
						<li><label :class="{'nav-active': activeNav == 6}" @click="activeNav = 6">{{ lang('uninstaller') }}</label></li>
						<li><label :class="{'nav-active': activeNav == 7}" @click="activeNav = 7">{{ lang('resources') }}</label></li>
						<button class="settings-btn" @click="activeNav = 8"></button>
					</ul>
					
					<dashboard v-show="activeNav == 1" />
					
					<system-cleaner v-show="activeNav == 2"/>

					<startup-apps v-show="activeNav == 3" />

					<services v-show="activeNav == 4" />

					<process v-show="activeNav == 5" />
		
					<uninstaller v-show="activeNav == 6" />

					<resources v-show="activeNav == 7" />

					<settings v-show="activeNav == 8" />

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
			activeNav: 5
		})
	},
	components: {
		'dashboard': Dashboard,
		'system-cleaner': SystemCleaner,
		'startup-apps': StartupApps,
		'services': Services,
		'process': Process,
		'uninstaller': Uninstaller,
		'resources': Resources,
		'settings': Settings,
	}
}