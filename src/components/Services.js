import { spawn } from 'child_process'
import sudo from 'sudo-prompt'
import { commands } from '../config'
import { command, showMessage } from '../helpers'

export default {
	template: `<div id="system-service-table">
				<div id="system-service-title">
					<span style="padding:0;">System Services ({{ filteredServices.length }})</span>
					<input type="text" id="system-service-search" v-model="searchString" placeholder="Search..." />
				</div>
				<div class="tdl-content scroll">
					<span class="fl w100 empty-list" v-show="! filteredServices.length" >
						No service found.
					</span>
					<ul v-show="filteredServices.length">
						<li v-for="service in filteredServices" >
							{{ service.name }}
							<input type="checkbox" class="switch" :id="service.name" :checked="service.isRun" @change="statusChange" />
							<label :for="service.name"></label>
						</li>
					</ul>
				</div>
			</div>`,
	data() {
		return ({
			servicesList: [],
			searchString: '',
			isBusy: false
		})
	},
	created() {
		const services = spawn('bash', ['-c', commands.getAllService])

		services.stdout.on('data', data => {
			data = data.toString().split('\n').filter( ( s ) => s != '')

			data.forEach( service => {
				var serviceName = service.substring(1)
				var isRun = service.substring(0, 1) == '+' ? 'checked' : ''
				this.servicesList.push( { name: serviceName, isRun: isRun } )
			})
		})
	},
	methods: {
        //Service status change switch button function         
        statusChange( e ) {
            let serviceName = e.target.id
            let status = e.target.checked ? 'start' : 'stop'

            if( ! this.isBusy ){
                this.isBusy = true
                sudo.exec( command( `service ${serviceName} ${status}` ) , {name: 'Stacer'},
                            (error, stdout, stderr) => {
                    if(stderr) {
						e.target.checked = ! status
                        showMessage( 'Operation not successful.', 'error')
					}
                    else {
                        showMessage( serviceName + ' service ' + status + (e.target.checked ? 'ed' : 'ped'), 'success')
					}

                    this.isBusy = false
                })
            } else {
                showMessage( 'Another process continues.' , 'error')
            }
        }
    },
    computed: {
        // Search services
        filteredServices() {
            let tempServicesList = this.servicesList ,
                searchString     = this.searchString

            if( ! searchString ) return tempServicesList

            searchString = searchString.toString().trim().toLowerCase()

            return tempServicesList.filter( item => 
                item.name.toString().trim().toLowerCase().indexOf(searchString) !== -1
            )
        }
    }
}