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
                    <slot></slot>
                    <div class="item-list uninstaller-list">
                        <div class="fl w100">
                            <h3 class="fl">{{ lang('uninstallerTitle') }} ({{ filteredPackages.length }})</h3>
                            <div class="spin-loader fl" v-show="isBusy">
                                <div class="dot1"></div>
                                <div class="dot2"></div>
                                <div class="dot3"></div>
                                <div class="dot4"></div>
                            </div>
                            <input type="text" v-model="searchString" :placeholder="lang('search') + '...'" />
                        </div>
                        <ul v-show="filteredPackages.length" class="scroll">
                            <li v-for="package in filteredPackages">
                                <input type ="checkbox" :value="package" :id="'p_' + package" v-model="selectedPackages"/>
                                <label :for="'p_' + package"></label>
                                {{ package }}
                            </li>
                        </ul>
                        <span class="empty-list" v-show="! filteredPackages.length">
                            {{ lang('noPackage') }}
                        </span>
                        <button :disabled="selectedPackages.length > 0 ? false : true" id="uninstall-selected" @click="uninstallSelected">
                            Uninstall Selecteds
                        </button>
                    </div>
                </div>
            </transition>`,
    data() {
        return ({
            packagesList: [],
            selectedPackages: [],
            searchString: '',
            isBusy: false
        })
    },
    created() {
        const packages = spawn('bash', ['-c', commands.getInstalledPackages])

        packages.stdout.on('data', data => {
            this.packagesList.splice(0, this.packagesList.length)
            data = data.toString().split('\n').filter(s => s != '')

            this.packagesList.push(...data)
        })
    },
    methods: {
        uninstallSelected() {
            if (this.selectedPackages) {
                if (!this.isBusy) {
                    this.isBusy = true

                    let sPackages = this.selectedPackages.join(' ')

                    sudo.exec(command(commands.removePackage + sPackages), {
                        name: 'Stacer'
                    },
                    (error, stdout, stderr) => {
                        if (stderr) {
                            showMessage(lang('uninstallFail'), 'error')
                        } else {
                            this.packagesList = this.packagesList
                                                    .filter(p => this.selectedPackages.indexOf(p) === -1)

                            showMessage(sPackages.split(' ').join('<br/>').concat('<br/>') + lang('packageUninstalled'), 'success')
                            this.searchString = ''
                        }
                        this.isBusy = false
                        this.selectedPackages = []
                    })
                } else {
                    showMessage(lang('anotherProc'), 'error')
                }
            }
        }
    },
    computed: {
        filteredPackages() {
            let tempPackagesList = this.packagesList,
                searchString = this.searchString

            if (!searchString) return tempPackagesList

            searchString = searchString.toString().trim().toLowerCase()

            return tempPackagesList.filter(item =>
                item.trim().toLowerCase().indexOf(searchString) !== -1
            )
        }
    }
}