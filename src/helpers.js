
import '../libs/amaran.min'

exports.prettyMemSize = ( size ) => {
  return ( size / ( Math.pow(1024, 3) ) ).toFixed(2)
}

exports.prettyDiskSize = ( size ) => {
  return ( size / Math.pow(1000, 3)).toFixed(1)
}

exports.showMessage = ( message , status) => {
    $.amaran({
        'theme'     :'colorful',
        'content'   :{
           bgcolor: status == 'success' ? '#27ae60' : '#c0392b',
           message: message
        },
        'position'  :'top right',
        'outEffect' :'slideTop'
    })
}

exports.command = ( command ) =>  `bash -c "${command}"`

