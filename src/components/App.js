import Dashboard from './dashboard/Dashboard'
import SystemCleaner from './system-cleaner/SystemCleaner'
import StartupApps from './startup-apps/StartupApps'
import Services from './services/Services'
import Process from './process/Process'
import Uninstaller from './uninstaller/Uninstaller'
import Resources from './resources/Resources'
import Settings from './settings/Settings'

export default {
	template: `<div id="main" class="fr">

						<div class="sidebar-nav">
							<ul>
								<li :class="isActiveNav(1)" @click="activeNav = 1" >
									<i class="icon-dash"></i>
									<label>{{ lang('dashboard') }}</label>
								</li>
								<li :class="isActiveNav(2)" @click="activeNav = 2" >
									<i class="icon-cleaner"></i>
									<label>{{ lang('sysCleaner') }}</label>
								</li>
								<li :class="isActiveNav(3)" @click="activeNav = 3" >
									<i class="icon-startup"></i>
									<label>{{ lang('startupApps') }}</label>
								</li>
								<li :class="isActiveNav(4)" @click="activeNav = 4" >
									<i class="icon-service"></i>
									<label>{{ lang('services') }}</label>
								</li>
								<li :class="isActiveNav(5)" @click="activeNav = 5" >
									<i class="icon-terminal"></i>
									<label>{{ lang('process') }}</label>
								</li>
								<li :class="isActiveNav(6)" @click="activeNav = 6" >
									<i class="icon-uninstaller"></i>
									<label>{{ lang('uninstaller') }}</label>
								</li>
								<li :class="isActiveNav(7)" @click="activeNav = 7" >
									<i class="icon-stats"></i>
									<label>{{ lang('resources') }}</label>
								</li>
								<li :class="isActiveNav(8)" @click="activeNav = 8" >
									<i class="icon-settings"></i>
									<label>{{ lang('appSettings') }}</label>
								</li>
							</ul>
						</div>

						<dashboard v-show="activeNav == 1">
							<h1> {{ lang('dashboard') }} </h1>
						</dashboard>
						
						<system-cleaner v-show="activeNav == 2">
							<h1> {{ lang('sysCleaner') }} </h1>
						</system-cleaner>

						<startup-apps v-show="activeNav == 3">
							<h1> {{ lang('startupApps') }} </h1>
						</startup-apps>

						<services v-show="activeNav == 4">
							<h1> {{ lang('services') }} </h1>
						</services>

						<process v-show="activeNav == 5">
							<h1> {{ lang('process') }} </h1>
						</process>
			
						<uninstaller v-show="activeNav == 6">
							<h1> {{ lang('uninstaller') }} </h1>
						</uninstaller>

						<resources v-show="activeNav == 7">
							<h1> {{ lang('resources') }} </h1>
						</resources>

						<settings v-show="activeNav == 8">
							<h1> {{ lang('appSettings') }} </h1>
						</settings>

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
			activeNav: 1
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
	},
	methods: {
		isActiveNav(val) {
				return this.activeNav == val ? 'nav-active' : ''
		}
	}
}