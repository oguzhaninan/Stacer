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
                        <div class="fl w100">
                            <h3 class="fl">{{ lang('uninstallerTitle') }} ({{ filteredPackages.length }})</h3>
                            <input type="text" v-model="searchString" :placeholder="lang('search') + '...'" />
                        </div>
                        <ul v-show="filteredPackages.length" class="scroll">
                            <li v-for="package in filteredPackages">
                                <div class="check">
                                    <input type ="checkbox" :value="package" :id="package" v-model="selectedPackages"/>
                                    <label :for="package"></label>
                                </div>
                                <span>{{ package }}</span>
                                <a :name="package" @click="removePackage"></a>
                            </li>
                        </ul>
                        <span class="empty-list" v-show="! filteredPackages.length">
                            {{ lang('noPackage') }}
                        </span>
                        <button v-show="selectedPackages.length" id="uninstall-selected" @click="uninstallSelected">
                            Uninstall Selecteds
                        </button>
                    </div>
                </div>
            </transition>`,
    data() {
        return ({
            packagesList: [],
            selectedPackages: [],
            isMultiUninstall: false,
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
        uninstallSelected() {
            console.log(this.selectedPackages.join(' '))
            if (this.selectedPackages) {
                if (!this.isBusy) {
                    this.isBusy = true

                    let sPackages = []
                    this.selectedPackages.forEach(pkg => sPackages.push(commands.removePackage + pkg))                   
                    console.log(sPackages.join('; '))
                    sudo.exec(command(sPackages.join('; ')), {
                                name: 'Stacer'
                            },
                            (error, stdout, stderr) => {
                                if (stderr) {
                                    showMessage(lang('uninstallFail'), 'error')
                                } else {
                                    this.searchString = ''
                                    
                                    this.selectedPackages.forEach(packageName => {
                                        var i = this.packagesList.indexOf(packageName)
                                        if (i != -1) this.packagesList.splice(i, 1)
                                    })

                                    showMessage(sPackages + lang('packageUninstalled'), 'success')
                                }
                                this.isBusy = false
                    })
                } else {
                    showMessage(lang('anotherProc'), 'error')
                }
                this.selectedPackages = []
            }
        },
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
                            showMessage(lang('uninstallFail'), 'error')
                        } else {
                            this.searchString = e.target.className = ''
                            var i = this.packagesList.indexOf(packageName)
                            if (i != -1) this.packagesList.splice(i, 1)

                            showMessage(packageName + lang('packageUninstalled'), 'success')
                        }
                        this.isBusy = false
                    })
            } else {
                showMessage(lang('anotherProc'), 'error')
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