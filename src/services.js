
import { spawn } from 'child_process'
import sudo from 'sudo-prompt'
import { commands } from './config'
import { command, showMessage } from './helpers'

let servicesList = [],
    isBusy = false

/**
 * Get system services
 */
const services = spawn('bash', ['-c', commands.getAllService])

services.stdout.on('data', ( data ) => {
    data = data.toString().split('\n').filter( ( s ) => s != '')

    data.forEach( ( service ) => {
        var serviceName = service.substring(1)
        var isRun = service.substring(0, 1) == '+' ? 'checked' : ''
        servicesList.push( { name: serviceName, isRun: isRun } )
    })
})


/**
 * Services component
 */
Vue.component('services', {
    template: '#services',
    data() {
        return { 
            services: servicesList, 
            searchString: ''
        }
    },
    methods: {
        /**
         * Service status change switch button function
         */
        statusChange( e ) {
            let serviceName = e.target.id
            let status  = e.target.checked ? 'start' : 'stop'

            if( ! isBusy ){
                isBusy = true
                sudo.exec( command( `service ${serviceName} ${status}` ) , {name: 'Stacer'},
                            (error, stdout, stderr) =>
                {
                    if(stderr)
                        showMessage( 'Operation not successful.', 'error')
                    else
                        showMessage( serviceName + ' service ' + status + (e.target.checked ? 'ed' : 'ped'), 'success')
                    
                    isBusy = false
                })
            }else {
                showMessage( 'Another process continues.' , 'error')
            }
        }
    },
    computed: {
        /**
         * Search services
         */
        filteredServices(){
            let tempServicesList = this.services,
                searchString     = this.searchString

            if( ! searchString ) return tempServicesList

            searchString = searchString.toString().trim().toLowerCase()

            return tempServicesList.filter( ( item ) => 
                item.name.toString().trim().toLowerCase().indexOf(searchString) !== -1
            )
        }
    }
})