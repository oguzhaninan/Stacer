
import si from 'systeminformation'
import { SemiCircle, Line } from 'progressbar.js'

import { prop } from './config'
import helpers  from './helpers'

let memInfo, dTotal, dUsed, down, up, information = []

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
exports.systemBars = () =>
{
  let memBar  = circleBar('#mem-cont', prop.memBarColor , prop.memDuration )

  let diskBar = circleBar('#disk-cont',prop.diskBarColor ,prop.diskDuration )

  // SETTERS // 
  /*
  * Cpu value setter
  */
 

  /*
  * Memory value setter
  */

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

