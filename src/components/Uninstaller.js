import { spawn } from 'child_process'
import sudo from 'sudo-prompt'
import { commands } from '../config'
import { command, showMessage } from '../helpers'

export default {
	template: `<div id="uninstaller-table">
				<div id="installed-packages-title">
					<span style="padding:0;">System Installed Packages ({{ filteredPackages.length }})</span>
					<input type="text" id="packages-search" v-model="searchString" placeholder="Search..." />
				</div>
				<div class="tdl-content scroll">
					<ul>
						<li v-for="package in filteredPackages" > {{ package }}
							<a :name="package" @click="removePackage"></a>
						</li>
					</ul>
				</div>
			</div>`,
	data() {
		return({			
			packagesList: [],
			searchString: '',
			isBusy: false
		})
	},
	created() {
		const packages = spawn('bash', ['-c', commands.getInstalledPackages])

		packages.stdout.on('data', data => {
			this.packagesList.splice(0, this.packagesList.length)
			data = data.toString().split('\n').filter((s) => s != '')

			this.packagesList.push(...data)
		})
	},
	methods: {
		removePackage(e) {
            if( ! this.isBusy ) {
                this.isBusy = true
                e.target.className += 'loader'
                let packageName = e.target.name

                sudo.exec( command( commands.removePackage + packageName ) , {name: 'Stacer'},
                        (error, stdout, stderr) =>
                {
                    if(stderr) {
                        e.target.className = ''
                        showMessage( 'Operation not successful.', 'error')
                    }
                    else {
                        this.searchString = e.target.className = ''
                        var i = this.packagesList.indexOf(packageName)
                        if(i != -1) this.packagesList.splice(i, 1)

                        showMessage( `${packageName} package uninstalled.`, 'success' )
                    }
                    this.isBusy = false
                })
            }
            else {
                showMessage( 'Another process continues.', 'error' )
            }
        }
	},
	computed: {
        filteredPackages() {
            let tempPackagesList = this.packagesList,
                searchString = this.searchString

            if (!searchString) return tempPackagesList

            searchString = searchString.toString().trim().toLowerCase()

            return tempPackagesList.filter( ( item ) =>
                item.trim().toLowerCase().indexOf(searchString) !== -1
            )
        }
    }
}