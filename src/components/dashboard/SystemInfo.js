import si from 'systeminformation'

export default {
  template: `<div class="system-info">
                <h3>{{ lang('systemInfo') }}</h3>
                <ul> <li v-for="info in information"> {{ info }} </li> </ul>
              </div>`,
  data() {
    return {
      information: []
    }
  },
  created() {
    si.osInfo(o => {
      this.information.push(lang('hostname') + ': ' + isUndefined(o.hostname))
      this.information.push(lang('platform') + ': ' + isUndefined(o.platform) + " " + isUndefined(o.arch))
      this.information.push(lang('distribution') + ': ' + isUndefined(o.distro) + " " + isUndefined(o.release))
      this.information.push(lang('kernelRel') + ': ' + isUndefined(o.kernel))
      si.cpu(c => {
        this.information.push(lang('cpuModel') + ': ' + isUndefined(c.manufacturer) + " " + isUndefined(c.brand))
        this.information.push(lang('cpuSpeed') + ': ' + isUndefined(c.speed) + "GHz")
        this.information.push(lang('cpuCores') + ': ' + c.cores)
      })
    })
  }
}

function isUndefined(o) {
  if(typeof o == 'string') return o
  else return 'Undefined'
}