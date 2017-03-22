
import '../../libs/amaran.min'

exports.prettyMemSize = ( size ) => {
  return ( size / ( Math.pow(1024, 3) ) ).toFixed(2)
}

exports.prettyDiskSize = ( size ) => {
  return ( size / Math.pow(1000, 3)).toFixed(1)
}

exports.showMessage = ( message , status ) => {
    let bgcolor
    switch (status) {
      case 'success':
        bgcolor = '#27ae60'; break;
      case 'error':
        bgcolor = '#c0392b'; break;
      default:
        bgcolor = '#f39c12'; break;
    }
    $.amaran({
        'theme'     :'colorful',
        'content'   :{
           bgcolor: bgcolor,
           message: message
        },
        'position'  :'top right',
        'outEffect' :'slideTop',
        'delay'     :5000
    })
}

exports.command = ( command ) =>  `bash -c "${command}"`

