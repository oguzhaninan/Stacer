import si from 'systeminformation'

export default {
	template: `<div class="system-info">SYSTEM INFO
                <ul> <li v-for="info in information"> {{ info }} </li> </ul>
              </div>`,
    data() {
      return {
          information: []
      }
    },
    created() {
      si.osInfo( o => {
          this.information.push( "Hostname: " + o.hostname )
          this.information.push( "Platform: " + o.platform + " " + o.arch )
          this.information.push( "Distribution: " + o.distro + " " + o.release )
          this.information.push( "Kernel Release: " + o.kernel )
          si.cpu( c => {
            this.information.push( "Cpu Model: " + c.manufacturer + " " + c.brand )
            this.information.push( "Cpu Speed: " + c.speed + "GHz" )
            this.information.push( "Cpu Cores: " + c.cores )
        })
      })
    }
}