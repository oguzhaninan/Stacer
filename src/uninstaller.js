import { spawn } from 'child_process'
import sudo  from 'sudo-prompt'
import { commands } from  './config'
import { command, showMessage } from './helpers'


let packagesList = [],
    isBusy = false

/**
 * Get installed packages
 */

const packages = spawn('bash', ['-c', commands.getInstalledPackages])

packages.stdout.on('data', (data) => {
    packagesList.splice(0, packagesList.length)    
    data = data.toString().split('\n').filter((s) => s != '')

    data.forEach( ( item ) =>
        packagesList.push(item)
    )
})


Vue.component('uninstaller', {
    template: '#uninstaller',
    data() {
        return {
            packagesList: packagesList,
            searchString: ''
        }
    },
    methods: {
        removePackage(e) {
            if( ! isBusy ) {
                isBusy = true
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
                        delete this.packagesList[ this.packagesList.indexOf(packageName) ]
                        showMessage( `${packageName} package uninstalled.`, 'success' )
                    }
                    isBusy = false
                })
            }
            else {
                showMessage( 'Another process continues.', 'success' )
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
})