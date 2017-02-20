
import si from 'systeminformation'
import { SemiCircle, Line } from 'progressbar.js'

import { prop } from './config'
import helpers  from './helpers'


let memInfo, dTotal, dUsed, down, up, information = []

/**
 * System info
 */
si.osInfo( ( o ) => {
    information.push( "Hostname: " + o.hostname )
    information.push( "Platform: " + o.platform + " " + o.arch )
    information.push( "Distribution: " + o.distro + " " + o.release )
    information.push( "Kernel Release: " + o.kernel )
    si.cpu( ( c ) => {
      information.push( "Cpu Model: " + c.manufacturer + " " + c.brand )
      information.push( "Cpu Speed: " + c.speed + "GHz" )
      information.push( "Cpu Cores: " + c.cores )
  })
})
/**
 * Create circle bar function
*/
function circleBar( id, color, duration ) {
  let cBar = new SemiCircle( id , {
    strokeWidth: 12,
    color: color,
    trailColor: prop.trailColor,
    easing: 'easeInOut',
    duration: duration,
    svgStyle: {
      marginTop: '10px'
    },
    text: {
      style: {
        color: '#fff',
        paddingBottom: '20px',
        position: 'absolute',
        left: '50%'
      }
    },
    step: ( state, bar ) =>
    {
      switch ( id )
      {
        case '#cpu-cont':
          bar.setText(Math.round(bar.value() * 100) + "%")
          break
        case '#mem-cont':
          bar.setText(memInfo)
          break
        case '#disk-cont':
          bar.setText(dUsed + "/" + dTotal + "GB")
          break
      }
    }
  })

  return cBar
}


exports.networkBars = ( ) =>
{
  /*
  * Create network down speed
  */
  let downBar = new Line('#down-bar', {
      strokeWidth: 5,
      easing: 'easeInOut',
      duration: prop.networkBarsDuration,
      color: prop.netwrokBarColor,
      trailColor: prop.trailColor,
      text: {
        style: {
            color: '#999',
            position: 'absolute',
            right: '0',
            marginTop: '5px'
        }
      },
      step: (state, bar) => {
          bar.setText(Math.abs(down).toString() + ' kB/s')
      }
  })

  /*
  * Create network upload speed
  */
  let upBar = new Line("#up-bar", {
      strokeWidth: 5,
      easing: 'easeInOut',
      duration: prop.networkBarsDuration,
      color: prop.netwrokBarColor,
      trailColor: prop.trailColor,
      text: {
        style: {
            color: '#999',
            position: 'absolute',
            right: '0',
            marginTop: '5px'
        }
      },
      step: ( state, bar ) => {
          bar.setText(Math.abs(up).toString() + ' kB/s')
      } 
  })

  /*
  * Network down and upload speed setter
  */
  setInterval( () => {
      si.networkInterfaceDefault( ( defaultNetwork ) => {
        setTimeout( () => {
          si.networkStats(defaultNetwork, ( data ) =>
          {
              down = (data.rx_sec / 1024).toFixed(2)
              up   = (data.tx_sec / 1024).toFixed(2)
              downBar.animate(down / 2000)
              upBar.animate(up / 2000)
          })
        }, 1000) })
  }, prop.networkBarsDuration)
  
}

exports.systemBars = () =>
{
  /*
  * Create the circle bars
  */
  let cpuBar  = circleBar('#cpu-cont', prop.cpuBarColor , prop.cpuDuration )

  let memBar  = circleBar('#mem-cont', prop.memBarColor , prop.memDuration )

  let diskBar = circleBar('#disk-cont',prop.diskBarColor ,prop.diskDuration )

  // SETTERS // 
  /*
  * Cpu value setter
  */
  setInterval( () => 
  {
    si.currentLoad( ( val ) => {
      cpuBar.animate(val.currentload / 100)
    })
  }, prop.cpuDuration)

  /*
  * Memory value setter
  */
  setInterval( () => {
    si.mem( ( ram ) => 
    {
      let usedMem  = ram.total - ram.available
      let totalMem = ram.total

      memInfo = helpers.prettyMemSize(usedMem) + ' / ' + helpers.prettyMemSize(totalMem) + 'GB'
      memBar.animate(usedMem / totalMem)
    })
  }, prop.memDuration);

  /*
  * Disk value setter
  */
  function diskInfo ()
  {
    si.fsSize( ( disk ) => {
      dUsed  = helpers.prettyDiskSize( disk[0].used )
      dTotal = helpers.prettyDiskSize( disk[0].size )
      diskBar.animate( disk[0].use / 100 )
    })
  }
  diskInfo()
  setInterval(diskInfo, prop.diskDuration)


}


/**
 * Components
 */
Vue.component('circle-bar', {
    template: '#circle-bar',
    props: ['text']
})

Vue.component('line-bar', {
    template: '#line-bar',
    props: ['text', 'name'] 
})    

Vue.component('system-info', {
    template: '#system-info',
    data() {
      return {
          information: information
      }
    }
})
