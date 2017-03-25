import {
    spawn
} from 'child_process'
import sudo from 'sudo-prompt'
import {
    commands
} from '../../utils/config'
import {
    command,
    showMessage
} from '../../utils/helpers'

export default {
    template: `<transition name="slide-fade">
                <div class="content">
                    <div class="item-list uninstaller-list">
                        <div>
                            <h3 class="fl">System Installed Packages ({{ filteredPackages.length }})</h3>
                            <input type="text" v-model="searchString" placeholder="Search..." />
                        </div>
                        <ul v-show="filteredPackages.length" class="scroll">
                            <li v-for="package in filteredPackages">
                                <span>{{ package }}</span>
                                <a :name="package" @click="removePackage"></a>
                            </li>
                        </ul>
                        <span class="empty-list" v-show="! filteredPackages.length" >
                            No package found.
                        </span>
                    </div>
                </div>
            </transition>`,
    data() {
        return ({
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
            if (!this.isBusy) {
                this.isBusy = true
                e.target.className += 'loader'
                let packageName = e.target.name

                sudo.exec(command(commands.removePackage + packageName), {
                        name: 'Stacer'
                    },
                    (error, stdout, stderr) => {
                        if (stderr) {
                            e.target.className = ''
                            showMessage('Operation not successful.', 'error')
                        } else {
                            this.searchString = e.target.className = ''
                            var i = this.packagesList.indexOf(packageName)
                            if (i != -1) this.packagesList.splice(i, 1)

                            showMessage(`${packageName} package uninstalled.`, 'success')
                        }
                        this.isBusy = false
                    })
            } else {
                showMessage('Another process continues.', 'error')
            }
        }
    },
    computed: {
        filteredPackages() {
            let tempPackagesList = this.packagesList,
                searchString = this.searchString

            if (!searchString) return tempPackagesList

            searchString = searchString.toString().trim().toLowerCase()

            return tempPackagesList.filter((item) =>
                item.trim().toLowerCase().indexOf(searchString) !== -1
            )
        }
    }
}